#include <stdint.h>
#include <stdlib.h>
#include "../include/shadow.h"
#include "../include/polynomial.h"


#define KMIN 3 
#define KMAX 8 

uint8_t ** generateShadows(uint8_t * secret, int secretLength, int k, int n) {

    int blockSize = 2*k - 2;
    int shadowSize = secretLength / (k-1);  
    if (k < KMIN || k > KMAX || secretLength % blockSize != 0)
        return NULL; 
    
    // Initial set-up: reserve space for the shadows and shadow array
    uint8_t ** shadows = (uint8_t **) malloc(sizeof(uint8_t*)*n); 
    for (int i=0; i<n; i++)
        shadows[i] = (uint8_t *) malloc(sizeof(uint8_t)*shadowSize);

    // dealer divides secret into t-non-overlapping 2k−2-pixel blocks, B1, B2, ..., Bt. 
    int shadowByteCounter = 0; 
    for (int i=0; i<secretLength; i+=blockSize) {
        Polynomial * fi = polyFromBytes(k, &secret[i]); 
        polyPrint(fi); 

        int ri = 3;  // TODO: replace by a random value 
        uint8_t bi0 = CONG(-1*ri*secret[i]); 
        uint8_t bi1 = CONG(-1*ri*secret[i+1]);
        Polynomial * gi = polyFromBytes(k, &secret[i+k-2]);
        gi->coefficients[0] = bi0; 
        gi->coefficients[1] = bi1; 

        polyPrint(gi); 

        for (int j=0; j<n; j++) {
            shadows[j][shadowByteCounter] = polyEvaluate(fi, j+1);
            shadows[j][shadowByteCounter+1] = polyEvaluate(gi, j+1);
        }

        shadowByteCounter += 2;  
        putchar('\n'); 

        polyFree(fi); 
        polyFree(gi); 
    }

    return shadows; 
}
