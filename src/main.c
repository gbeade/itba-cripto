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

    uint8_t * secret = (uint8_t *)"GONZALO"; 

    // Declare parameters of the schema
    int n = 4;
    int k = 3; 

    // Considers the NULL-terminated
    int secretLength; 
    for (secretLength=1; secret[secretLength-1]; secretLength++);

    int shadowSize = secretLength / (k-1); 


    int ids[3] = {1, 2, 3}; 
    uint8_t ** shadows = generateShadows(secret, secretLength, k, n);
    uint8_t * secretReconstructed = reconstruct(shadows, ids, shadowSize, k);
    
    printf("The schema is (%d, %d).\n", k, n);
    printf("The secret %s\n\n", secret); 

    printf("%d shadows were created. They are:\n", n);
    for (int i=0; i<n; i++) {
        printf("S%d -> ", i+1);
        for (int j=0; j<shadowSize; j++) {
            printf("0x%2X ", shadows[i][j]); 
        }
        putchar('\n'); 
    }
    putchar('\n');

    printf("We will now combine the first %d shadows.\n", k);

    printf("The reconstructed secret is: %s\n", secretReconstructed); 

}

int main() {
    tryShadowGeneration(); 
    return 0;
}