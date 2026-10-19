// cpuFreqTest.c      TDT4258 2026 code example RPi4-5  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

void printCPUfrequency(){
    fflush(stdout); // needed to ensure in-order-printing because stdout is buffered and the 
        // system call (the shell command cat) below writes directly to stdout bypassing its buffer 
    system("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"); // prints frequency of first cpu core
} // scaling_cur_freq is a virtual file exposed by the linux kernel and reports the currently used frequency

void printGovernor(){
    fflush(stdout); // flush needed for same reason as above. 
    system("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
}

void performanceTest(int scale) {  // Just a simple ad hoc loop using time, taking approx 1 second on 900 MHz
    long long accum = 0;           // Parameter scale is used to multiply the time consumption. Scale == 1 is approx 1 second
    for (int i = 0; i < 100*scale; i++) {
        for (int j = 0; j < 1000; j++) {
            for (int k = 0; k < 1000; k++) {
                accum++;
            }
        }
    }
    printf("Result: %lld \n", accum);    
    return;
}

int main(int argc, char *argv[]) {
    int scale = argc > 1 ? atoi(argv[1]) : 1; // scale 1 is defaultvalue
    if (scale < 1) scale = 1;
    printf("Using governor: "); printGovernor();
    printf("Starts with CPU frequency: "); printCPUfrequency();
    clock_t start = clock();
    performanceTest(scale);
    printf("Execution time: %.3f ms\n", 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
    printf("Finished-at-CPU-frequency: "); printCPUfrequency();
    return 0;
}