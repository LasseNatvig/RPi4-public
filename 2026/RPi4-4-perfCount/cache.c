// cache.c - Cache-related functions for TDT4258 code example RPi4-4
#include "cache.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int iterations = 400;  // Magic number, gives exec time at approx 1 second for cache - sequential
int* arr = NULL;

int* genArray(long size) { // Allocates an array of integers, all with value = 1
    int* arr = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {arr[i] = 1;}
    return arr;
}
volatile uint64_t sink; // Used as output sink to prevent the compiler eliminating loops

//Sequential read — stride-1, hardware prefetcher works well
void sequential(const int *arr, int n) {
    uint64_t s = 0;
    for (int it = 0; it < iterations; it++) {
        for (int i = 0; i < n; i++) { 
            s += arr[i];
        }
    }
    sink = s;
    // printf("sink= %lld\n", sink); // debug
}

// Strided read — reduced performance
// This function with stride == 1 gives the same cache access-pattern as the the function sequential,
//  but it takes approx 3 times longer, probably due to the index-calculation
void strided(const int *arr, int n, int stride) {
    uint64_t s = 0;
    for (int it = 0; it < iterations; it++) {
        for (int i = 0; i < n; i++) {
            s += arr[(i * stride) % n];
        }
    }
    sink = s;
    // printf("sink= %lld\n", sink); // debug
}

// The following two function should test the branch prediction hw, but was not
// included to avoid making the example too big.

// /* 3. Predictable branch — condition is always true (arr[i] >= 0) */
// static void wl_predictable(const int *arr, int n)
// {
//     uint64_t s = 0;
//     for (int it = 0; it < ITERATIONS; it++)
//         for (int i = 0; i < n; i++)
//             if (arr[i] >= 0) s += arr[i];   /* always taken */
//     sink = s;
// }

// /* 4. Unpredictable branch — alternates ~50/50 on odd/even values */
// static void wl_unpredictable(const int *arr, int n)
// {
//     uint64_t s = 0;
//     for (int it = 0; it < ITERATIONS; it++)
//         for (int i = 0; i < n; i++)
//             if (arr[i] & 1) s += arr[i];    /* ~50% taken */
//     sink = s;
// }