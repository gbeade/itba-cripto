#include <stdio.h>
#include "include/cuTest.h"
#include "../include/steganography.h"


void testHidelsb4(CuTest *const cuTest) {
    uint8_t secret[4] = {0b11111111, 0b00110100, 0b00010010, 0b00100011 };
    uint8_t original[8] = {0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00011111 };
    lsb4Hide(secret, 4, original);
    uint8_t answer[8] = {0b00011111, 0b00011111, 0b00010011, 0b00010100, 0b00010001, 0b00010010, 0b00010010, 0b00010011 };
    for (int i = 0 ; i< 8 ; i++) {
        CuAssertIntEquals(cuTest, original[i], answer[i]);
    }
}

void testHidelsb2(CuTest *const cuTest) {
    uint8_t secret[2] = {0b11111111, 0b00110100};
    uint8_t original[8] = {0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00011111 };
    lsb2Hide(secret, 4, original);
    uint8_t answer[8] = {0b00010011,0b00010011, 0b00010011, 0b00010011, 0b00010000, 0b00010011, 0b00010001, 0b00011100 };
    for (int i = 0 ; i< 8 ; i++) {
        CuAssertIntEquals(cuTest, original[i], answer[i]);
    }
}

void testHideAndShowlsb4(CuTest *const cuTest) {
    uint8_t secret[8] = {10, 24, 32, 65, 0xf3, 0x3f, 0xdd, 0xAE};
    uint8_t original[16] = {12, 34, 45, 56, 67, 78, 100, 0xf3, 0xff, 100, 0xde, 0xf3, 0xff, 100, 0xde, 0xff};
    lsb4Hide(secret, 8, original);
    uint8_t maybeSecret[8];
    lsb4Show(original, 8, maybeSecret);
    for (int i = 0 ; i< 8 ; i++) {
        CuAssertIntEquals(cuTest, secret[i], maybeSecret[i]); 
    }
}

void testHideAndShowlsb2(CuTest *const cuTest) {
    uint8_t secret[4] = {10, 24, 32, 65};
    uint8_t original[16] = {12, 34, 45, 56, 67, 78, 100, 0xf3, 0xff, 100, 0xde, 0xf3, 0xff, 100, 0xde, 0xff};
    lsb2Hide(secret, 4, original);
    uint8_t maybeSecret[4];
    lsb2Show(original, 4, maybeSecret);
    for (int i = 0 ; i< 4 ; i++) {
        CuAssertIntEquals(cuTest, secret[i], maybeSecret[i]); 
    }
}

