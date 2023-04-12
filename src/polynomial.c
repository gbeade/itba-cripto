#include "../include/polynomial.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

Polynomial* polyFromCoefficients(unsigned int n, ...) {
    va_list args;
    va_start(args, n);
    
    Polynomial* poly = (Polynomial*)malloc(sizeof(Polynomial));
    poly->degree = n - 1;
    poly->coefficients = (uint8_t*)malloc(n * sizeof(uint8_t));
    
    for (unsigned i = 0; i < n; i++) {
        uint8_t coeff = (uint8_t)(va_arg(args, uint32_t) % MOD);
        poly->coefficients[i] = coeff;
    }
    
    va_end(args);
    
    return poly;
}

uint8_t polyEvaluate(Polynomial* poly, unsigned int x) {
    uint32_t result = 0;
    uint32_t power = 1;
    
    for (int i = 0; i <= poly->degree; i++) {
        result += (uint32_t)poly->coefficients[i] * power;
        power = (power * x) % MOD;
        result = result % MOD; 
    }
    
    return (uint8_t)(result);
}

void polyPrint(Polynomial* poly) {
    printf("p(x) = ");
    
    printf("%d", poly->coefficients[0]);
    for (int i = 1; i < poly->degree+1; i++) 
        printf(" + %dx^%d", poly->coefficients[i], i);
    
    printf("\n");
}

void polyFree(Polynomial* poly) {
    free(poly->coefficients);
    free(poly);
}
