// helpers.c -  - Helper functions for TDT4258 code example RPi4-4 performance counters
#include "helpers.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Demonstrates also variable argument list, just as we have in printf
void pBoth(FILE *outfile, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args); //Print to stdout

    // Reset args for reuse (required for vfprintf)
    va_end(args);
    va_start(args, format);
    // Print to file if outfile is not NULL
    if (outfile) {
        vfprintf(outfile, format, args);
    }
    va_end(args);
    fflush(stdout); fflush(outfile);
}

// Helper function to get current timestamp as string
void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y%m%d-%H%M%S", tm_info);
}

// Helper function to create and open an output file
bool createOutputFile(char *filename, size_t size, FILE **outfile) {
    char timestamp[20];
    get_timestamp(timestamp, sizeof(timestamp));
    snprintf(filename, size, "res/res-%s.txt", timestamp);
    *outfile = fopen(filename, "w");
    if (!*outfile) {
        fprintf(stderr, "Error: Could not open output file %s\n Program will exit", filename);
        return false ;
    }
    return true;
}

// Helper function to parse command line arguments
void parseArgs(int argc, char *argv[], char *expName, int *E, int *start, int *step, int *end) {
    if (argc > 1) {
        strncpy(expName, argv[1], 255);
        expName[255] = '\0';
    }
    if (argc > 2) {
        *E = atoi(argv[2]);
        if (*E <= 0) {
            fprintf(stderr, "Error: E must be a positive integer\n");
            exit(1);
        }
    }
    if (argc > 3) {
        *start = atoi(argv[3]);
        if (*start <= 0) {
            fprintf(stderr, "Error: start must be a positive integer\n");
            exit(1);
        }
    }
    if (argc > 4) {
        *step = atoi(argv[4]);
        if (*step <= 0) {
            fprintf(stderr, "Error: step must be a positive integer\n");
            exit(1);
        }
    }
    if (argc > 5) {
        *end = atoi(argv[5]);
        if (*end <= 0) {
            fprintf(stderr, "Error: end must be a positive integer\n");
            exit(1);
        }
    }
}
