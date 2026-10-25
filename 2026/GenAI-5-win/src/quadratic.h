// quadratic.h - Quadratic equation solver header - Example code for TDT4258 - 2026 edition
// Initially written by Cline and MistralMedium

#ifndef QUADRATIC_H
#define QUADRATIC_H

/**
 * Solves quadratic equation ax^2 + bx + c = 0
 * @param a coefficient of x^2
 * @param b coefficient of x
 * @param c constant term
 * @param x1 pointer to first solution
 * @param x2 pointer to second solution
 * @return number of real solutions (0, 1, or 2)
 */
int solveQuadratic(double a, double b, double c, double *x1, double *x2);

#endif