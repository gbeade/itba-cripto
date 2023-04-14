#ifndef SHADOW_H
#define SHADOW_H
#include <stdint.h>

uint8_t ** generateShadows(uint8_t * secret, int secretLength, int k, int n);

/* returns NULL if cheating */
uint8_t ** reconstruct(uint8_t ** shadows, int k, int * ids);

#endif // SHADOW_H
