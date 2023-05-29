#include <stdio.h>
#include <stdlib.h>
#include "../include/shadow.h"

#include "include/cuTest.h"

void testShadowGeneration(CuTest *const cuTest) {

    /* Secret that we expect to recover */
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
    
    CuAssertIntEquals(cuTest, secretReconstructed[0], 'G');
    CuAssertIntEquals(cuTest, secretReconstructed[1], 'O');
    CuAssertIntEquals(cuTest, secretReconstructed[2], 'N');
    CuAssertIntEquals(cuTest, secretReconstructed[3], 'Z');
    CuAssertIntEquals(cuTest, secretReconstructed[4], 'A');
    CuAssertIntEquals(cuTest, secretReconstructed[5], 'L');
    CuAssertIntEquals(cuTest, secretReconstructed[6], 'O');
    CuAssertIntEquals(cuTest, secretReconstructed[7], '\0');

    for (int i=0; i<n; i++) {
        free(shadows[i]);
    }
    free(shadows); 
    free(secretReconstructed); 
}

void testShadowGenerationMod(CuTest *const cuTest) {

    uint8_t secret[] = {249, 250, 251, 252, 253, 254, 255, 0}; 

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
    
    CuAssertIntEquals(cuTest, secretReconstructed[0], 249);
    CuAssertIntEquals(cuTest, secretReconstructed[1], 250);
    CuAssertIntEquals(cuTest, secretReconstructed[2], 0);
    CuAssertIntEquals(cuTest, secretReconstructed[3], 1);
    CuAssertIntEquals(cuTest, secretReconstructed[4], 2);
    CuAssertIntEquals(cuTest, secretReconstructed[5], 3);
    CuAssertIntEquals(cuTest, secretReconstructed[6], 4);
    CuAssertIntEquals(cuTest, secretReconstructed[7], 0);

    for (int i=0; i<n; i++) {
        free(shadows[i]);
    }
    free(shadows); 
    free(secretReconstructed); 
}

void verbose(CuTest *const cuTest) {
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
    
    CuAssertIntEquals(cuTest, 1, 1);
    
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

        for (int i=0; i<n; i++) {
        free(shadows[i]);
    }
    free(shadows); 
    free(secretReconstructed); 

}



    