#include "../include/steganography.h"
#include <limits.h>
#include <libc.h>
#include <arpa/inet.h>

#define LSB(x,n) ( ((x) >> (n) << (n)) ^ (x) ) 
#define MSB(x,n) ( (x) >> (CHAR_BIT - (n) ) )
#define SET_LSBS_TO_ZERO(x, n) ( (x) >> (n) << (n) ) 

static uint8_t * hideNextByte(uint8_t *originalBytes, uint8_t byteToHide, uint8_t bitsPerByte) {
    uint8_t * byteCarrierLimit = originalBytes + (CHAR_BIT / bitsPerByte);
    while (originalBytes < byteCarrierLimit) {
        uint8_t carrierByte = *originalBytes;
        carrierByte = SET_LSBS_TO_ZERO(carrierByte, bitsPerByte);
        uint8_t bitsToHide = MSB(byteToHide, bitsPerByte);
        carrierByte = carrierByte | bitsToHide;
        *originalBytes = carrierByte;
        byteToHide <<= bitsPerByte;
        originalBytes++;
    }
    return originalBytes;
}


static uint8_t * lsbHide(uint8_t * secret, int secretLength, uint8_t * original, int bitsPerByte) {
    for (int i = 0 ; i < secretLength ; i++) {
        original = hideNextByte(original, secret[i], bitsPerByte);
    }
    return original;
}



static uint8_t extractNextByte(uint8_t ** offuscatedBytesPtr, uint8_t bitsPerByte) {
    uint8_t byte = 0;
    uint8_t * offuscatedBytes = *offuscatedBytesPtr;
    uint8_t * byteCarrierLimit = offuscatedBytes + (CHAR_BIT / bitsPerByte);
    while (offuscatedBytes < byteCarrierLimit) {
        uint8_t carrierByte = *offuscatedBytes;
        uint8_t hiddenBits = LSB(carrierByte, bitsPerByte);
        byte <<= bitsPerByte;
        byte |= hiddenBits;
        offuscatedBytes++;
    }
    *offuscatedBytesPtr = offuscatedBytes;
    return byte;
}

uint8_t * lsbShow(uint8_t * offuscated, int secretLength, int bitsPerByte, uint8_t * secretOutput) {
    int retrievedBytes = 0;
    while(retrievedBytes < secretLength) {
        secretOutput[retrievedBytes++] = extractNextByte(&offuscated, bitsPerByte);
    }
    return secretOutput;
}

uint8_t * lsb2Hide(uint8_t * secret, int secretLength, uint8_t * original) {
    return lsbHide(secret, secretLength, original, 2);
}

uint8_t * lsb2Show(uint8_t * offuscated, int secretLength, uint8_t * secretOutput) {
    return lsbShow(offuscated, secretLength, 2, secretOutput);
}

uint8_t * lsb4Hide(uint8_t * secret, int secretLength, uint8_t * original)  {
    return lsbHide(secret, secretLength, original, 4);
}

uint8_t * lsb4Show(uint8_t * offuscated, int secretLength, uint8_t * secretOutput) {
    return lsbShow(offuscated, secretLength, 4, secretOutput);
}
