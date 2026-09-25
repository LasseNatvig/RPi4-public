// report.c - Reporting functions for TDT4258 code example RPi4-4 performance counters
#include "report.h"
#include "helpers.h"
#include "stats.h"
#include "measure.h"

// Function to calculate and print timing statistics
bool printTiming(FILE *outfile, double times[], int E) {
    // Calc timing statistics
    double time_mean = calc_mean(times, E);
    double time_std_dev = calc_std_dev(times, E, time_mean);
    double time_rel_std_dev = calc_rel_std_dev(times, E, time_mean);
    double time_min = calc_min(times, E);
    double time_max = calc_max(times, E);

    // Print timing results
    pBoth(outfile, "\n=== Timing Summary ===\n");
    pBoth(outfile, "  Mean:   %.6f seconds\n", time_mean);
    pBoth(outfile, "  StdDev: %.6f seconds\n", time_std_dev);
    pBoth(outfile, "  RelStd: %.2f%%\n", time_rel_std_dev);
    pBoth(outfile, "  Min:    %.6f seconds\n", time_min);
    pBoth(outfile, "  Max:    %.6f seconds\n", time_max);
    
    pBoth(outfile, "\n  Times: ");
    for (int i = 0; i < E; i++) {
        pBoth(outfile, "%.6f", times[i]);
        if (i < E - 1) pBoth(outfile, ", ");
    }
    pBoth(outfile, "\n");
    return true;
}

// Function to calculate and print performance counter statistics 
void printCounterStats(FILE *outfile, uint64_t** counterVals, int experiments) {
    calcCounterStats(counterVals, experiments);
    // fprintf(outfile, "\n=== Counter Statistics ===\n");
    pBoth(outfile, "\n=== Counter Statistics ===\n");
    for (int c = 0; c < NUM_COUNTERS; c++) {
        if (c == IDX_CYCLES || c == IDX_INSTRUCTIONS) {
            pBoth(outfile, "  %s: mean=%.2fM", counterNames[c], counterMeans[c] / 1000000.0);
        } else if (c == IDX_CACHE_REFS) {
            pBoth(outfile, "  %s: mean=%.2fk", counterNames[c], counterMeans[c] / 1000.0);
        } else {
            pBoth(outfile, "  %s: mean=%.0f", counterNames[c], counterMeans[c]);
        }
        pBoth(outfile, " (%.2f%%)\n", counterRelStdDevs[c]);
    }
    
    // Calculate and print cache miss rate
    if (counterMeans[IDX_CACHE_REFS] > 0) {
        double cacheMissRate = counterMeans[IDX_CACHE_MISSES] / counterMeans[IDX_CACHE_REFS];
        pBoth(outfile, "  cacheMissRate: %.4f\n", cacheMissRate);
    }
    
    // Calculate and print IPC (Instructions Per Cycle)
    if (counterMeans[IDX_CYCLES] > 0) {
        double ipc = counterMeans[IDX_INSTRUCTIONS] / counterMeans[IDX_CYCLES];
        pBoth(outfile, "  IPC: %.4f\n", ipc);
    }
    
    // Calculate and print branch misses per million instructions
    if (counterMeans[IDX_INSTRUCTIONS] > 0) {
        double branchMissesPerMillion = (counterMeans[IDX_BRANCH_MISSES] * 1000000.0) / counterMeans[IDX_INSTRUCTIONS];
        pBoth(outfile, "  BranchMissesPerMillionInstructions: %.2f\n", branchMissesPerMillion);
    }
}
