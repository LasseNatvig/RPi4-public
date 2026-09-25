// measure.h - Performance measurement infrastructure for ARM Cortex-A72, for course TDT4258
// Compatible with Raspberry Pi 4 (ARM Cortex-A72)
// Code has been developed on an RPi4 with vsCode & Cline running Mistral Medium on Idun@NTNU,
// see the README file.

#ifndef MEASURE_H
#define MEASURE_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Performance counter indices
#define IDX_CYCLES 0
#define IDX_INSTRUCTIONS 1
#define IDX_CACHE_REFS 2
#define IDX_CACHE_MISSES 3
#define IDX_BRANCH_MISSES 4
#define NUM_COUNTERS 5

extern const char* counterNames[NUM_COUNTERS];

// Structure to hold counter values
struct perf_counter {
    uint64_t value;
    int fd;
};

// Structure to hold measurement results
struct measurement_result {
    double time_seconds;
    uint64_t cycles;
    uint64_t instructions;
    uint64_t cache_refs;
    uint64_t cache_misses;
    uint64_t branch_misses;
    double ipc;
    double cache_miss_percent;
    double branch_miss_per_instr;
};

// Function declarations
void counters_init(void);
void counters_start(void);
void counters_stop(void);
void counters_cleanup(void);

// Global counter array (external definition)
extern struct perf_counter ctrs[NUM_COUNTERS];

// External declarations for counter statistics
extern double counterMeans[NUM_COUNTERS];
extern double counterRelStdDevs[NUM_COUNTERS];

#endif // MEASURE_H