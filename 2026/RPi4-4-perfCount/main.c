// main.c in TDT4258 2026 code example RPi4-4
// Performance measurement setup with performance counters
// Runs experiment E times and reports timing statistics
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include "measure.h"
#include "stats.h"
#include "helpers.h"
#include "report.h"
#include "primes.h"
#include "cache.h"
#include "sort.h"

struct timespec start, end;  // for measuring time
double* times = NULL; // pointer to execution times array
uint64_t** counterVals = NULL; // pointer to array for storing performance counter values
void allocateMeasurementsStorage(int experiments) {
    times = malloc(experiments * sizeof(double));
    counterVals = malloc(NUM_COUNTERS * sizeof(uint64_t *));
    if (!times || !counterVals) {
        fprintf(stderr, "Failed to allocate memory\n");
        free(times); free(counterVals);
        counters_cleanup();
        exit(-1);
    }
    for (int c = 0; c < NUM_COUNTERS; c++) {
        counterVals[c] = malloc(experiments * sizeof(uint64_t));
        if (!counterVals[c]) {
            fprintf(stderr, "Failed to allocate memory\n");
            for (int d = 0; d < c; d++) free(counterVals[d]);
            free(counterVals); free(times);
            counters_cleanup();
            exit(-1);
        }
    }
}
void freeMeasurementsStorage() {
    free(times); //    // Cleanup
    for (int c = 0; c < NUM_COUNTERS; c++) free(counterVals[c]);
    free(counterVals);
    counters_cleanup();
}
void startExperiment() {
    clock_gettime(CLOCK_MONOTONIC, &start);       
    counters_start(); 
}
void endExperiment(int exp) {
    counters_stop();
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    times[exp] = elapsed;
    for (int c = 0; c < NUM_COUNTERS; c++) { counterVals[c][exp] = ctrs[c].value; }
    // pBoth(outfile, "Experiment %d/%d: %.6f seconds\n", exp + 1, E, elapsed); // debug
}

int main(int argc, char *argv[]) {
    char outputFilename[256]; // Reports both to terminal and a timestamped file
    FILE *outfile = NULL;
    createOutputFile(outputFilename, sizeof(outputFilename), &outfile);
    pBoth(outfile, "Performance Measurements Demo\nOutput file: %s\n", outputFilename);

    char expName[256] = "none"; // default experiment name
    int E = 1, start = 0, step = 0, end = 0; // default argument values
    parseArgs(argc, argv, expName, &E, &start, &step, &end);
    // Check if expName is valid
    if ((strcmp(expName, "prime") == 0) || (strcmp(expName, "cache") == 0) || (strcmp(expName, "sorting") == 0)) {
        pBoth(outfile, "Experiment: %s\n", expName);
    } else {
        pBoth(outfile, "experiment with name %s not available\n", expName);
        freeMeasurementsStorage();
        fclose(outfile);
        exit(-1);
    }    
    pBoth(outfile, "Arguments E, start, step, end : %d,  %d, %d, %d\n", E, start, step, end);
    
    allocateMeasurementsStorage(E);
    counters_init(); // start using perf counters

    if ((strcmp(expName, "prime") == 0)) {
        // pBoth(outfile, "Running %d experiments...\n", E);  // debug
        for (int exp = 0; exp < E; exp++) {
            startExperiment();
            long result = nth_prime(100000);  // correct is 1299709
            endExperiment(exp);
            // pBoth(outfile, "10000th prime is: %d\n", result); // debug 
        }
        printTiming(outfile, times, E); // Calculate and print timing results
        printCounterStats(outfile, counterVals, E); // ... and some performance counters statistics
    }

    if ((strcmp(expName, "cache") == 0)) {
        long size = 1000000;
        int* arr = NULL;
        arr = genArray(size);
        pBoth(outfile, "\nCache-sequential:\n");
        for (int exp = 0; exp < E; exp++) {
            startExperiment();
            sequential(arr, size);
            endExperiment(exp);
        }
        printTiming(outfile, times, E); // Calculate and print timing results
        printCounterStats(outfile, counterVals, E); 

        for (int stride = start; stride <= end; stride += step) {
            pBoth(outfile, "\nCache-strided: stride= %d\n", stride);
            for (int exp = 0; exp < E; exp++) {
                startExperiment();
                strided(arr, size, stride);
                endExperiment(exp);
            }
            printTiming(outfile, times, E); // Calculate and print timing results
            printCounterStats(outfile, counterVals, E); 
        }
    }

    if ((strcmp(expName, "sorting") == 0)) {
        for (int size = start; size <= end; size += step) {
            int* arr = malloc(size * sizeof(int));
            if (!arr) {
                fprintf(stderr, "Failed to allocate memory for array\n");
                freeMeasurementsStorage();
                fclose(outfile);
                exit(-1);
            }

            pBoth(outfile, "\ninsertionSort: size= %d", size);
            for (int exp = 0; exp < E; exp++) {
                srand(0);
                generateRandomArray(arr, size);
                startExperiment();
                insertionSort(arr, size);
                endExperiment(exp);
            }
            printTiming(outfile, times, E); // Calculate and print timing results
            printCounterStats(outfile, counterVals, E); 
            pBoth(outfile, "\nbubbleSort: size= %d", size);
            for (int exp = 0; exp < E; exp++) {
                srand(0);
                generateRandomArray(arr, size);
                startExperiment();
                bubbleSort(arr, size);
                endExperiment(exp);
            }
            printTiming(outfile, times, E); // Calculate and print timing results
            printCounterStats(outfile, counterVals, E); 
            free(arr);
        }
    }
    
    freeMeasurementsStorage();
    pBoth(outfile, "\nResults saved to: %s\n", outputFilename);
    fclose(outfile);
    return 0;
}