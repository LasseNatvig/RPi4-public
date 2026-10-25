// all_tests.c - Test runner for all test suites - Example code for TDT4258 - 2026 edition
// Initially written by Cline and MistralMedium

#include "unity.h"
#include <stdio.h>

// Declare all test functions
extern void testTwoRealSolutions(void);
extern void testOneRealSolution(void);
extern void testNoRealSolutions(void);
extern void testLinearEquation(void);
extern void testStandardQuadratic(void);
extern void testNegativeCoefficients(void);
extern void testInvalidEquationAllZero(void);
extern void testLinearEquationZeroConstant(void);
extern void testAllCoefficientsZero(void);
extern void testDiscriminantNearZero(void);
extern void testLargeCoefficients(void);
extern void testSmallCoefficients(void);

int main(void) {
    // Run quadratic solver tests
    RUN_TEST(testTwoRealSolutions);
    RUN_TEST(testOneRealSolution);
    RUN_TEST(testNoRealSolutions);
    RUN_TEST(testLinearEquation);
    RUN_TEST(testStandardQuadratic);
    RUN_TEST(testNegativeCoefficients);
    RUN_TEST(testInvalidEquationAllZero);
    RUN_TEST(testLinearEquationZeroConstant);
    RUN_TEST(testAllCoefficientsZero);
    RUN_TEST(testDiscriminantNearZero);
    RUN_TEST(testLargeCoefficients);
    RUN_TEST(testSmallCoefficients);
 
    // Print summary
    printf("\nTest Summary:\n");
    printf("Total tests: %d\n", unityGetTestCount());
    printf("Failed: %d\n", unityGetFailCount());
    printf("Ignored: %d\n", unityGetIgnoreCount());

    return unityGetFailCount();
}