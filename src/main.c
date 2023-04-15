#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp.h"
#include "../include/polynomial.h"
#include "../include/steganography.h"
#define MSB(x,n) ( (x) >> (8 - (n) ) )

void printBits(void const * const ptr, size_t const size)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
}

void printBytes(uint8_t * vec, int bytes) {
    for (int i=0 ; i<bytes ; i++) {
        printBits(vec+i, 1);
    }
    puts("");
}

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
    int ys[N] = {3, 7, 13}; 


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
    tryLagrange(); 
    return 0;
}