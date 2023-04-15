#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H
#include <stdint.h>

uint8_t * lsb2Hide(uint8_t * secret, int secretLength, uint8_t * original);

uint8_t * lsb2Show(uint8_t * offuscated, int secretLength, uint8_t * secretOutput);

uint8_t * lsb4Hide(uint8_t * secret, int secretLength, uint8_t * original);

uint8_t * lsb4Show(uint8_t * offuscated, int secretLength, uint8_t * secretOutput);

#endif // STEGANOGRAPHY_H
