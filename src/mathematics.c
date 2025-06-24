#ifndef MATHEMATICS_C
#define MATHEMATICS_C

#include <stdio.h>
#include <stdlib.h>

long int factorial(int n) {
    long int ret;

    if (n < 0) {
        fprintf(stderr, "Attempting factorial of negative number");
        exit(EXIT_FAILURE);
    } else if (n == 0) {
        return 1;
    } else {
        for (int i = (n-1); i > 0; i--) {
            ret *= i;
        }
    }

    return ret;
}

long int combination(int n, int k) {
    if (k > n) {
        return 0;
    } else {
        return factorial(n)/(factorial(k) * factorial(n-k));
    }
}


#endif // !MATHEMATICS_C
