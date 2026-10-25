// parallelPi.c  used in TDT4258 2026 code example RPi4-5 
// 
// Usage: ./parallelPi <threads 1-4> [steps]
// Pi = integral of 4/(1+x^2) over [0,1], midpoint rule, steps split evenly over the threads.
// using POSIX thread (pthreads)
// code made by AI and it is NOT part of you reading list

#define _GNU_SOURCE  // pthread_setaffinity_np
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>

#define MAX_THREADS 4

typedef struct {
    int id;
    long long start, end;
    double step;
    double sum;
} Work;

static void *integrate(void *arg) {
    Work *w = arg;

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(w->id, &set);  // one thread per core
    pthread_setaffinity_np(pthread_self(), sizeof(set), &set);

    double sum = 0.0;
    for (long long i = w->start; i < w->end; i++) {
        double x = (i + 0.5) * w->step;
        sum += 4.0 / (1.0 + x * x);
    }
    w->sum = sum;
    return NULL;
}

int main(int argc, char *argv[]) {
    int nThreads = argc > 1 ? atoi(argv[1]) : 1;
    long long steps = argc > 2 ? atoll(argv[2]) : 400000000LL; // long is 32-bit on this armhf userland
    if (nThreads < 1 || nThreads > MAX_THREADS || steps < nThreads) {
        fprintf(stderr, "Usage: %s <threads 1-%d> [steps]\n", argv[0], MAX_THREADS);
        return EXIT_FAILURE;
    }

    double step = 1.0 / steps;
    pthread_t threads[MAX_THREADS];
    Work work[MAX_THREADS];

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (int t = 0; t < nThreads; t++) {
        work[t] = (Work){ .id = t, .start = steps * t / nThreads, .end = steps * (t + 1) / nThreads, .step = step };
        pthread_create(&threads[t], NULL, integrate, &work[t]);
    }
    double pi = 0.0;
    for (int t = 0; t < nThreads; t++) {
        pthread_join(threads[t], NULL);
        pi += work[t].sum;
    }
    pi *= step;

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double ms = (t1.tv_sec - t0.tv_sec) * 1e3 + (t1.tv_nsec - t0.tv_nsec) / 1e6;

    printf("Threads: %d, Pi = %.12f, Execution time: %.1f ms\n", nThreads, pi, ms);
    return EXIT_SUCCESS;
}
