// main.c - Quadratic equation solver demo - Example code for TDT4258 - 2026 edition
// Initially written by Cline and MistralMedium
//
// a short main program to demonstrate the use of the quadratic solver function


#include <stdio.h>
#include "src/quadratic.h"

int main() {
    double a, b, c;
    double x1, x2;
    int numSolutions;

    printf("Quadratic Equation Solver: ax^2 + bx + c = 0\n\n");
    
    // Example: x^2 - 5x + 6 = 0 (solutions: 2 and 3)
    a = 1; b = -5; c = 6;

    numSolutions = solveQuadratic(a, b, c, &x1, &x2);

    printf("Equation: %.1fx^2 + %.1fx + %.1f = 0\n", a, b, c);
    printf("Number of real solutions: %d\n", numSolutions);

    if (numSolutions == 2) {
        printf("Solutions: x1 = %.2f, x2 = %.2f\n", x1, x2);
    } else if (numSolutions == 1) {
        printf("Solution: x = %.2f\n", x1);
    } else {
        printf("No real solutions\n");
    }

    return 0;
}