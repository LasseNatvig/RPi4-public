// helpers.h - Helper functions for TDT4258 code example RPi4-4 performance counters
#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdbool.h>

// Prints a formatted string to both stdout and an optional file stream.
 
void pBoth(FILE *outfile, const char *format, ...);

// Gets the current timestamp as a formatted string.
void get_timestamp(char *buffer, size_t size);

// Creates and opens an output file with a timestamp.
bool createOutputFile(char *filename, size_t size, FILE **outfile);

/* 
 * Parses command line arguments for expName, E, start, step, end.
 * Updates the provided variables based on argv.
 * Uses default values if arguments are not provided.
 */
void parseArgs(int argc, char *argv[], char *expName, int *E, int *start, int *step, int *end);

#endif // HELPERS_H
