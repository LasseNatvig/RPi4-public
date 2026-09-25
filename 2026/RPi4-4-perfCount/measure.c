// measure.c - Performance measurement implementation for ARM Cortex-A72
// Uses perf_event_open() for hardware performance counters

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/perf_event.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>

#include "measure.h"

// Global counter array
struct perf_counter ctrs[NUM_COUNTERS];

// Performance event types for ARM Cortex-A72
// Using raw event codes for ARM Cortex-A72 PMU (from /sys/devices/platform/pmu/armv8_cortex_a72/events/)
static int event_types[NUM_COUNTERS] = {
    0x0011,  // cpu_cycles (PERF_COUNT_HW_CPU_CYCLES)
    0x0008,  // inst_retired (PERF_COUNT_HW_INSTRUCTIONS)
    0x0004,  // l1d_cache (PERF_COUNT_HW_CACHE_REFERENCES)
    0x0003,  // l1d_cache_refill (PERF_COUNT_HW_CACHE_MISSES)
    0x0010   // br_mis_pred (PERF_COUNT_HW_BRANCH_MISSES)
};

const char* counterNames[NUM_COUNTERS] = {
    "Cycles", "Instructions", "Cache refs", "Cache misses", "Branch misses"
};

// Initialize performance counters
void counters_init(void) {
    struct perf_event_attr attr;  // structure used in system calls to define the use of performance counters
    memset(&attr, 0, sizeof(attr));
    
    // Use PERF_TYPE_RAW for ARM Cortex-A72 raw event codes
    attr.type = PERF_TYPE_RAW;
    attr.size = sizeof(attr);
    attr.config = 0; // Will be set per counter
    attr.exclude_kernel = 1;  // exclude kernel space events, we want to measure the user-space code
    attr.exclude_hv = 1; // excludes hypervisor events, probably not relevant forRPi4
    attr.disabled = 1; // counters are initially turned off (disabled), we want to start all at the same point
    attr.inherit = 1; // needed to measure multithreaded applications, not used in the code in this folder
    
    for (int i = 0; i < NUM_COUNTERS; i++) { // opens one performance counter per event type
        attr.config = event_types[i];  // it creates a file descriptor for each performance counter
        ctrs[i].fd = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0); // linux system call. 0 means current process
                            // -1 means all CPUs, but effectievly binds to current CPU context (according to Cline&MM)
        if (ctrs[i].fd < 0) {  // Suggests a common workaround for permission issues
            fprintf(stderr, "Error opening performance counter %d (event 0x%X): %s\n", i, event_types[i], strerror(errno));
            fprintf(stderr, "Try: echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid\n");
            exit(EXIT_FAILURE);
        }
        ctrs[i].value = 0;
    }
}

// Start all counters
void counters_start(void) { // ioctl() operates on file descriptors to control the performance counters
    // Reset all counters to zero
    for (int i = 0; i < NUM_COUNTERS; i++) {
        if (ioctl(ctrs[i].fd, PERF_EVENT_IOC_RESET, 0) < 0) {
            fprintf(stderr, "Error resetting counter %d: %s\n", i, strerror(errno));
        }
    }
    // Enable all counters
    for (int i = 0; i < NUM_COUNTERS; i++) {
        if (ioctl(ctrs[i].fd, PERF_EVENT_IOC_ENABLE, 0) < 0) {
            fprintf(stderr, "Error enabling counter %d: %s\n", i, strerror(errno));
        }
    }
}

// Stop all counters and read values
void counters_stop(void) {
    // Read values from all counters first (before disabling)
    for (int i = 0; i < NUM_COUNTERS; i++) {
        ssize_t ret = read(ctrs[i].fd, &ctrs[i].value, sizeof(uint64_t));
        if (ret != sizeof(uint64_t)) {
            fprintf(stderr, "Error reading counter %d: %s (ret=%zd)\n", i, strerror(errno), ret);
        }
    }
    // Disable all counters
    for (int i = 0; i < NUM_COUNTERS; i++) {
        if (ioctl(ctrs[i].fd, PERF_EVENT_IOC_DISABLE, 0) < 0) {
            fprintf(stderr, "Error disabling counter %d: %s\n", i, strerror(errno));
        }
    }
}

// Cleanup counters
void counters_cleanup(void) {  // close all the open performance counter file descriptors 
    for (int i = 0; i < NUM_COUNTERS; i++) {
        if (ctrs[i].fd >= 0) {
            close(ctrs[i].fd);
            ctrs[i].fd = -1;
        }
    }
}
