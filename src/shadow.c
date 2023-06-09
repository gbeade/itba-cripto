#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/shadow.h"
#include "../include/polynomial.h"

#define KMIN 3
#define KMAX 8

uint8_t** generateShadows(uint8_t* secret, int secretLength, int k, int n) {

    int blockSize = 2 * k - 2;
    int shadowSize = secretLength / (k - 1);
    if (k < KMIN || k > KMAX || secretLength % (k - 1) != 0)
        return NULL;

    // Initial set-up: reserve space for the shadows and shadow array
    uint8_t** shadows = (uint8_t**)malloc(sizeof(uint8_t*) * n);
    for (int i = 0; i < n; i++)
        shadows[i] = (uint8_t*)malloc(sizeof(uint8_t) * shadowSize);

    // dealer divides secret into t-non-overlapping 2k−2-pixel blocks, B1, B2, ..., Bt.
    int shadowByteCounter = 0;
    for (int i = 0; i < secretLength; i += blockSize) {
        int ai0Backup = secret[i];
        int ai1Backup = secret[i + 1];

        secret[i] = CONG(secret[i]) ? secret[i] : 1;
        secret[i + 1] = CONG(secret[i + 1]) ? secret[i + 1] : 1;

        Polynomial* fi = polyFromBytes(k, &secret[i]);
        int ri = CONG(1 + rand() % (MOD - 1));

        uint8_t bi0 = CONG(-1 * ri * secret[i]);
        uint8_t bi1 = CONG(-1 * ri * secret[i + 1]);

        Polynomial* gi = polyFromBytes(k, &secret[i + k - 2]);
        gi->coefficients[0] = bi0;
        gi->coefficients[1] = bi1;

        // Restore ai0 and ai1 to avoid modifying original image
        secret[i] = ai0Backup;
        secret[i + 1] = ai1Backup;

        for (int j = 0; j < n; j++) {
            shadows[j][shadowByteCounter] = polyEvaluate(fi, j + 1);
            shadows[j][shadowByteCounter + 1] = polyEvaluate(gi, j + 1);
        }

        shadowByteCounter += 2;

        polyFree(fi);
        polyFree(gi);
    }
    return shadows;
}

static int isCheating(int ai0, int ai1, int bi0, int bi1) {
    for (int i = 0; i < MOD; i++) {
        if (CONG(ai0 * i + bi0) == 0 && CONG(ai1 * i + bi1) == 0) {
            return 0;
        }
    }
    return 1;
}

uint8_t* reconstruct(uint8_t** shadows, int* ids, int shadowLength, int k) {

    int secretLength = shadowLength * (k - 1);
    int blockSize = 2 * k - 2;
    uint8_t* secret = (uint8_t*)malloc(sizeof(uint8_t) * secretLength);

    int currentBlock = 0;
    for (int i = 0; i < shadowLength; i += 2) {
        int ys1[k];
        int ys2[k];
        for (int j = 0; j < k; j++) {
            ys1[j] = shadows[j][currentBlock * 2];
            ys2[j] = shadows[j][currentBlock * 2 + 1];
        }

        Polynomial* fi = polyInterpolate(k, ids, ys1);
        Polynomial* gi = polyInterpolate(k, ids, ys2);

        int ai0 = fi->coefficients[0];
        int ai1 = fi->coefficients[1];
        int bi0 = gi->coefficients[0];
        int bi1 = gi->coefficients[1];

        // Check for Cheating
        if (isCheating(ai0, ai1, bi0, bi1)) {
            polyFree(fi);
            polyFree(gi);
            free(secret);
            return NULL;
        }

        for (int t = 0; t < k; t++)
            secret[currentBlock * blockSize + t] = fi->coefficients[t];

        for (int t = 2; t < k; t++)
            secret[currentBlock * blockSize + k + t - 2] = gi->coefficients[t];

        polyFree(fi);
        polyFree(gi);
        currentBlock++;
    }

    return secret;
}