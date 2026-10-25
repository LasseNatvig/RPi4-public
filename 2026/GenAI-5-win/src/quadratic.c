// quadratic.c - Quadratic equation solver implementation - Example code for TDT4258 - 2026 edition
// Initially written by Cline and MistralMedium

#include "quadratic.h"
#include <math.h>

int solveQuadratic(double a, double b, double c, double *x1, double *x2) {
    if (a == 0) {
        if (b == 0) {
            return 0; // Not a quadratic equation
        }
        *x1 = -c / b;
        return 1; // Linear equation
    }

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return 0; // No real solutions
    }

    if (discriminant == 0) {
        *x1 = -b / (2 * a);
        return 1; // One real solution
    }

    *x1 = (-b + sqrt(discriminant)) / (2 * a);
    *x2 = (-b - sqrt(discriminant)) / (2 * a);
    return 2; // Two real solutions
}