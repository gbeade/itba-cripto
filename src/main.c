#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp.h"
#include "../include/polynomial.h"


void tryBmp(char * path) {
    BMPImage* bmp = loadBmp(path);
    dumpBmpInverted(bmp); 
    freeBmp(bmp); 
}

void tryPolynomial() {
    printf("\nBeginning test poly\n"); 

    // Create polynomial from coefficients
    Polynomial * poly = polyFromCoefficients(3, 1, -1, 1); 

    // Print polynomial
    polyPrint(poly); 

    // Evaluate the polyomial on sample value
    int x = 200; 
    int t = polyEvaluate(poly, x); 
    printf("p(%d)=%d\n", x, t);

    polyFree(poly); 
}

#define N 3
void tryLagrange() {
    printf("\nBeginning test lagrange\n"); 

    
    int xs[N] = {1, 2, 3}; 
    int ys[N] = {1, 2, 3}; 


    // Create polynomial from coefficients
    Polynomial * poly = polyInterpolate(N, xs, ys); 

    // Print polynomial
    polyPrint(poly); 

    for (int i = 0; i<N; i++) {
        int t = polyEvaluate(poly, xs[i]); 
        printf("p(%d)=%d\n", xs[i], t);
    }


    polyFree(poly); 
}


int main() {
    tryPolynomial(); 
    return 0;
}