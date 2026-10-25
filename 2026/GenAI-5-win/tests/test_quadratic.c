// test_quadratic.c - Unit tests for quadratic solver - Example code for TDT4258 - 2026 edition
// Initially written by Cline and MistralMedium

#include "unity.h"
#include "../src/quadratic.h"
#include <math.h>

// Helper macro for floating point comparison with tolerance
#define TEST_ASSERT_EQUAL_DOUBLE(expected, actual) \
    if (fabs((expected) - (actual)) > 0.0001) { \
        unityFail("Assert Double Equal Failed"); \
        return; \
    }

TEST_CASE(testTwoRealSolutions) {
    double x1, x2;
    int result = solveQuadratic(1, -5, 6, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(2, result);
    TEST_ASSERT_EQUAL_DOUBLE(3.0, x1);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, x2);
}

TEST_CASE(testOneRealSolution) {
    double x1, x2;
    int result = solveQuadratic(1, -4, 4, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, x1);
}

TEST_CASE(testNoRealSolutions) {
    double x1, x2;
    int result = solveQuadratic(1, 0, 1, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(0, result);
}

TEST_CASE(testLinearEquation) {
    double x1, x2;
    int result = solveQuadratic(0, 2, 4, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_DOUBLE(-2.0, x1);
}

TEST_CASE(testStandardQuadratic) {
    double x1, x2;
    int result = solveQuadratic(1, -3, 2, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(2, result);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, x1);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, x2);
}

TEST_CASE(testNegativeCoefficients) {
    double x1, x2;
    int result = solveQuadratic(-1, 3, -2, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(2, result);
    // For -x^2 + 3x - 2 = 0, solving gives: x = [ -3 +/- sqrt(1) ] / (-2)
    // x1 = (-3 + 1) / (-2) = (-2) / (-2) = 1
    // x2 = (-3 - 1) / (-2) = (-4) / (-2) = 2
    TEST_ASSERT_EQUAL_DOUBLE(1.0, x1);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, x2);
}

// Edge case: a=0, b=0, c!=0 - not a valid equation
TEST_CASE(testInvalidEquationAllZero) {
    double x1, x2;
    int result = solveQuadratic(0, 0, 5, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// Edge case: a=0, b!=0, c=0 - linear equation with zero constant
TEST_CASE(testLinearEquationZeroConstant) {
    double x1, x2;
    int result = solveQuadratic(0, 2, 0, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, x1);
}

// Edge case: all coefficients zero
TEST_CASE(testAllCoefficientsZero) {
    double x1, x2;
    int result = solveQuadratic(0, 0, 0, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// Boundary case: discriminant very close to zero
TEST_CASE(testDiscriminantNearZero) {
    double x1, x2;
    // Use values that result in solutions very close to -1
    // b^2 - 4ac = 1e-10 (very small positive)
    // For x^2 + 2x + (1 - 2.5e-11) = 0
    // discriminant = 4 - 4*(1 - 2.5e-11) = 1e-10
    // sqrt(1e-10) = 1e-5
    // Solutions: x = (-2 +/- 1e-5) / 2 = -1 +/- 5e-6
    double c_val = 1 - 2.5e-11;
    int result = solveQuadratic(1, 2, c_val, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(2, result);
    // Solutions should be very close to -1 +/- 5e-6
    TEST_ASSERT_EQUAL_DOUBLE(-1.0 + 5e-6, x1);
    TEST_ASSERT_EQUAL_DOUBLE(-1.0 - 5e-6, x2);
}

// Stress test: very large coefficients
TEST_CASE(testLargeCoefficients) {
    double x1, x2;
    int result = solveQuadratic(1e10, 2e10, 1e10, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_DOUBLE(-1.0, x1);
}

// Stress test: very small coefficients
TEST_CASE(testSmallCoefficients) {
    double x1, x2;
    int result = solveQuadratic(1e-10, 2e-10, 1e-10, &x1, &x2);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_DOUBLE(-1.0, x1);
}