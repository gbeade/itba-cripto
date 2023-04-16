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

        int ri = 3;  // TODO: replace by a random value
         
        uint8_t bi0 = CONG(-1*ri*secret[i]); 
        uint8_t bi1 = CONG(-1*ri*secret[i+1]);
        Polynomial * gi = polyFromBytes(k, &secret[i+k-2]);
        gi->coefficients[0] = bi0; 
        gi->coefficients[1] = bi1; 


        for (int j=0; j<n; j++) {
            shadows[j][shadowByteCounter] = polyEvaluate(fi, j+1);
            shadows[j][shadowByteCounter+1] = polyEvaluate(gi, j+1);
        }

        shadowByteCounter += 2;  

        polyFree(fi); 
        polyFree(gi); 
    }

    return shadows; 
}


uint8_t * reconstruct(uint8_t ** shadows, int * ids, int shadowLength, int k) {

    int secretLength = shadowLength*(k-1); 
    int blockSize = 2*k - 2;
    uint8_t * secret = (uint8_t *) malloc(sizeof(uint8_t)*secretLength); 

    int currentBlock = 0; 
    for (int i=0; i<shadowLength; i+=2) {
        int ys1[k];
        int ys2[k]; 
        for (int j=0; j<k; j++) {
            ys1[j] = shadows[j][currentBlock*2];
            ys2[j] = shadows[j][currentBlock*2+1];  
        }

        Polynomial * fi = polyInterpolate(k, ids, ys1);
        Polynomial * gi = polyInterpolate(k, ids, ys2);

        // TODO: check for cheating

        for (int t=0; t<k; t++) 
            secret[currentBlock*blockSize+t] = fi->coefficients[t];
        
        for (int t=2; t<k; t++) 
            secret[currentBlock*blockSize+k+t-2] = gi->coefficients[t];
        

        polyFree(fi); 
        polyFree(gi); 
        currentBlock ++; 
    }

    return secret; 


}
