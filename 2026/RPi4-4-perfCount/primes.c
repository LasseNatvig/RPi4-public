// primes.c
// Prime number calculation functions

#include "primes.h"
#include <stdbool.h>
#include <math.h>

// Function to check if a number is prime
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

// Function to calculate the nth prime number
int nth_prime(int n) {
    if (n < 1) return -1;
    
    int count = 0;
    int candidate = 2;
    
    while (count < n) {
        if (is_prime(candidate)) {
            count++;
        }
        candidate++;
    }
    
    return candidate - 1;
}
