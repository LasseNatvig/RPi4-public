// stats.h - Statistical calculation functions
#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <stdio.h>

// Function declarations for basic statistics
double calc_mean(double *array, int size);
double calc_std_dev(double *array, int size, double mean);
double calc_rel_std_dev(double *array, int size, double mean);
double calc_min(double *array, int size);
double calc_max(double *array, int size);

// Function declarations for uint64_t array statistics
uint64_t calc_mean_uint64(uint64_t *array, int size);
double calc_std_dev_uint64(uint64_t *array, int size, double mean);
double calc_rel_std_dev_uint64(uint64_t *array, int size, double mean);
uint64_t calc_min_uint64(uint64_t *array, int size);
uint64_t calc_max_uint64(uint64_t *array, int size);

void calcCounterStats(uint64_t **counter_values, int E);

#endif // STATS_H
