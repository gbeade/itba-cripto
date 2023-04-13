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

Polynomial* polyInterpolate(int n, int* xs, int* ys) {
   
    Polynomial* poly = (Polynomial*) malloc(sizeof(Polynomial));
    poly->degree = n - 1;
    poly->coefficients = (uint8_t*) malloc(n * sizeof(uint8_t));

    int qc = 0; // Number of coefficients already analysed
    int yPrimes[n];  // y' cache - the y' are constructed recursively as interpolation progresses
    while (qc < n) {
        int currentCoefficient = 0;

        // Reduced Lagrange algorithm. In each iteration we ignore one extra point
        int top = n-qc;
        for (int i = 0; i<top; i++) {

            // Now we are standing on a particular (xs[i], ys[i]) point
            // We need to calculate y' and Li(0) so that we can contribute to 
            // currentCoefficient by adding Li(0)*y'

            // I) Calculate y' -> Use Ana's trick for converting y to y'
            int y = !qc ? ys[i] : (yPrimes[i] - poly->coefficients[qc-1]) * INV(xs[i]); 
            y = CONG(y);
            yPrimes[i] = y; 

            // II) Calculate Li(0)
            int li = 1; 
            for (int j=0; j<top; j++) 
                li *= i == j ? 1 : CONG(-1*xs[j]*INV(xs[i]-xs[j]));
            
            // Sum to the current coefficient 
            currentCoefficient += CONG(y*li);
        }

        poly->coefficients[qc++] = (uint8_t)CONG(currentCoefficient); 
    }


    return poly; 
}
