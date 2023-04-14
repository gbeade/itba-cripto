#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp.h"
#include "../include/polynomial.h"
#include "../include/shadow.h"



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


void tryShadowGeneration() {

    printf("\nBeginning test shadow generation\n------\n"); 

    uint8_t * secret = (uint8_t *)"GONZALO!"; 

    int secretLength = 0; 
    for (; secret[secretLength]; secretLength++);
    printf("len of secret: %d\n\n", secretLength);  

    int n = 4;
    int k = 3; 
    int shadowSize = secretLength / (k-1); 

    // print shadow as numbers 
    for (int j=0; j<secretLength; j++) {
        printf("[%d]", secret[j]); 
    }

    printf("\n\n"); 
    printf("n=%d\nk=%d\nshadowSize=%d\n\n", n, k, shadowSize);
    uint8_t ** shadows = generateShadows(secret, secretLength, k, n);


    for (int i=0; i<n; i++) {
        printf("SHADOW %d: \n", i); 
        for (int j=0; j<shadowSize; j++) {
            printf("%d ", shadows[i][j]); 
        }
        printf("\n\n"); 
    }

    


}

int main() {
    tryShadowGeneration(); 
    return 0;
}