#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp.h"
#include "../include/polynomial.h"
#include "../include/shadow.h"
#include "../include/steganography.h"
#include "../include/img.h"


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
    int n = 5;
    int k = 3; 

    // Considers the NULL-terminated
    int secretLength; 
    for (secretLength=1; secret[secretLength-1]; secretLength++);

    int shadowSize = secretLength / (k-1); 
    uint8_t originals[n][secretLength];

   

    int ids[3] = {1, 2, 3}; 
    uint8_t ** shadows = generateShadows(secret, secretLength, k, n);

     for (int i=0 ; i<n ; i++) {
        lsb4Hide(shadows[i], shadowSize, originals[i]);
        lsb4Show(originals[i], shadowSize, shadows[i]);
    }

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

void tryBmpShadow(char * path) {
    BMPMap* bmpMap = newBmpMap(path);
    BMPImage * bmp = mapToBmpImage(bmpMap); 

    printf("\nBeginning test bmp+shadow generation\n------\n"); 

    uint8_t * secret = bmp->data; 

    // Declare parameters of the schema
    int n = 5;
    int k = 3; 

    // Considers the NULL-terminated
    int secretLength = 512*512; 

    int shadowSize = secretLength / (k-1); 


    int ids[3] = {1, 2, 3}; 
    uint8_t ** shadows = generateShadows(secret, secretLength, k, n);
    uint8_t * secretReconstructed = reconstruct(shadows, ids, shadowSize, k);
    
    printf("The schema is (%d, %d).\n", k, n);
    printf("The secret is the bmp at %s\n\n", path); 

    printf("%d shadows were created. They are:\n", n);
    for (int i=0; i<n; i++) {
        printf("S%d -> ", i+1);
        for (int j=0; j<10; j++) {
            printf("0x%2X ", shadows[i][j]); 
        }
        printf("...\n"); 
    }
    putchar('\n');

    int t = 10; // 512*512; 
    printf("We will now combine the first %d shadows.\n", k);
    printf("Do the first %d bytes match?\n", t); 

    for (int i=0; i<t; i++) 
        printf("[0x%2X] ~ [0x%2X] %4s\n", secret[i], secretReconstructed[i], secret[i] == secretReconstructed[i] ? "OK": "FAIL");


    for (int i=0; i<n; i++)
        free(shadows[i]); 
    free(shadows); 
    free(secretReconstructed); 
    freeBmpImage(bmp); 
    freeBmpMap(bmpMap); 
}

void tryBmp(char * path) {
    BMPMap * bmpMap1 = newBmpMap(path);
    BMPImage * bmpImage1 = mapToBmpImage(bmpMap1);

    BMPMap * bmpMap2 = newBmpMap(path);
    BMPImage * bmpImage2 = mapToBmpImage(bmpMap2); 

    uint8_t * data = malloc(sizeof(uint8_t)*512*512); 
    for (int i=0; i<512*512; i++) 
            data[i] = (i+512)%7 ? 255 : 0;   

    BMPImage * bmp = bytesToBmpImage((uint8_t *)bmpImage2->header, data); 
    dumpBmpToFile(bmp, "bin/out.bmp"); 

    freeBmpImage(bmpImage1); 
    freeBmpImage(bmpImage2); 
    freeBmpImage(bmp); 
    free(data); 

    freeBmpMap(bmpMap1); 
    freeBmpMap(bmpMap2); 
}

void tryComplete() {
    distribute("samples/shadows", "samples/sample.bmp", 3); 
}

int main() {
    tryComplete(); 
    return 0;
}