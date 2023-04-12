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
    Polynomial * poly = polyFromCoefficients(3, 1, 1, 1); 

    // Print polynomial
    polyPrint(poly); 

    // Evaluate the polyomial on sample value
    int x = 200; 
    int t = polyEvaluate(poly, x); 
    printf("p(%d)=%d\n", x, t);

    polyFree(poly); 
}

int main() {
    printf("Multiplicative inverses in modulo %d->%d\n", 20, INV(20));
    printf("Multiplicative inverses in modulo %d->%d\n", -20, INV(-20));


    return 0;
}