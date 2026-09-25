// stats.c - Statistical calculation functions
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "stats.h"
#include "measure.h"

// Function to calc mean of an array
double calc_mean(double *array, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}

// Function to calc standard deviation of an array
double calc_std_dev(double *array, int size, double mean) {
    double sum_sq = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = array[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / size);
}

// Function to calc relative standard deviation (percentage)
double calc_rel_std_dev(double *array, int size, double mean) {
    if (mean == 0.0) return 0.0;
    double std_dev = calc_std_dev(array, size, mean);
    return (std_dev / mean) * 100.0;
}

// Function to calc min value in array
double calc_min(double *array, int size) {
    double min_val = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] < min_val) {
            min_val = array[i];
        }
    }
    return min_val;
}

// Function to calc max value in array
double calc_max(double *array, int size) {
    double max_val = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max_val) {
            max_val = array[i];
        }
    }
    return max_val;
}

// Function to calc mean of uint64_t array
uint64_t calc_mean_uint64(uint64_t *array, int size) {
    uint64_t sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}

// Function to calc standard deviation of uint64_t array
double calc_std_dev_uint64(uint64_t *array, int size, double mean) {
    double sum_sq = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = (double)array[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / size);
}

// Function to calc relative standard deviation for uint64_t array
double calc_rel_std_dev_uint64(uint64_t *array, int size, double mean) {
    if (mean == 0.0) return 0.0;
    double std_dev = calc_std_dev_uint64(array, size, mean);
    return (std_dev / mean) * 100.0;
}

// Function to calc min value in uint64_t array
uint64_t calc_min_uint64(uint64_t *array, int size) {
    uint64_t min_val = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] < min_val) {
            min_val = array[i];
        }
    }
    return min_val;
}

// Function to calc max value in uint64_t array
uint64_t calc_max_uint64(uint64_t *array, int size) {
    uint64_t max_val = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max_val) {
            max_val = array[i];
        }
    }
    return max_val;
}


double counterMeans[NUM_COUNTERS];
double counterRelStdDevs[NUM_COUNTERS];

// Function to compute all counter statistics
void calcCounterStats(uint64_t **counter_values, int E) {
    for (int c = 0; c < NUM_COUNTERS; c++) {
        double *values = malloc(E * sizeof(double));
        for (int i = 0; i < E; i++) {
            values[i] = (double)counter_values[c][i];
        }
        counterMeans[c] = calc_mean(values, E);
        counterRelStdDevs[c] = calc_rel_std_dev(values, E, counterMeans[c]);

        free(values);
    }
}



