// unity.c - Unity test framework implementation - Example code for TDT4258 - 2026 edition
// Initially written by Cline and MistralMedium

#include "unity.h"
#include <stdio.h>
#include <string.h>

static int failedCount = 0;
static int testCount = 0;
static int ignoreCount = 0;

void unityTestStart(const char* file, uint32_t line) {
    printf("TEST: %s:%u\n", file, line);
    testCount++;
}

void unityTestEnd(void) {
}

void unityFail(const char* msg) {
    printf("FAIL: %s\n", msg);
    failedCount++;
}

void unityIgnore(void) {
    ignoreCount++;
}

int unityGetFailCount(void) {
    return failedCount;
}

int unityGetTestCount(void) {
    return testCount;
}

int unityGetIgnoreCount(void) {
    return ignoreCount;
}