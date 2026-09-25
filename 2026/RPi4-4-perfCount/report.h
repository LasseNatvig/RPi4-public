// report.h - Reporting functions for TDT4258 code example RPi4-4 performance counters
#ifndef REPORT_H
#define REPORT_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool printTiming(FILE *outfile, double times[], int E);
void printCounterStats(FILE *outfile, uint64_t** counterVals, int E);

#endif // REPORT_H