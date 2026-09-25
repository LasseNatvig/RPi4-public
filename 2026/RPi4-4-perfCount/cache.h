// cache.h - Cache-related functions for TDT4258 code example RPi4-4
#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>

// Function to allocate an array of integers
int* genArray(long size);

// Sequential read — stride-1, hardware prefetcher works well
void sequential(const int *arr, int n);

// Strided read — reduces performance, demonstrates performance effect of stride 
void strided(const int *arr, int n, int stride);

#endif // CACHE_H
