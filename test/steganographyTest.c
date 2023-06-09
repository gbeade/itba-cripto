#include <stdio.h>
#include "include/cuTest.h"
#include "../include/steganography.h"


void testHidelsb4(CuTest *const cuTest) {
    uint8_t secret[4] = {0xFF, 0x34, 0x12, 0x23 };
    uint8_t original[8] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };
    lsb4Hide(secret, 4, original);
    uint8_t answer[8] = {0x1F, 0x1F, 0x13, 0x14, 0x11, 0x12, 0x12, 0x13 };
    for (int i = 0 ; i< 8 ; i++) {
        CuAssertIntEquals(cuTest, original[i], answer[i]);
    }
}

void testHidelsb2New2(CuTest *const cuTest) {
    uint8_t secret[1] = {0xda};
    uint8_t original[4] = {0x77, 0x77, 0x76, 0x74 };
    lsb2Hide(secret, 1, original);
    uint8_t answer[4] = {0x77, 0x75, 0x76, 0x76 };
    for (int i = 0 ; i< 4 ; i++) {
        CuAssertIntEquals(cuTest, original[i], answer[i]);
    }
}

void testShowlsb4(CuTest *const cuTest) {
    uint8_t offuscated[8] = {0x1B, 0x1E, 0x1A, 0x1D, 0x1E, 0x1B, 0x1E, 0x1A};
    uint8_t hide[4]; 

    lsb4Show(offuscated, 4, hide);

    CuAssertIntEquals(cuTest, hide[0], 0xBE);
    CuAssertIntEquals(cuTest, hide[1], 0xAD);
    CuAssertIntEquals(cuTest, hide[2], 0xEB);
    CuAssertIntEquals(cuTest, hide[3], 0xEA);
}


void testHidelsb2(CuTest *const cuTest) {
    uint8_t secret[2] = {0xFF, 0x34};
    uint8_t original[8] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };
    lsb2Hide(secret, 2, original);
    uint8_t answer[8] = {0x13, 0x13, 0x13, 0x13, 0x10, 0x13, 0x11, 0x1C };
    for (int i = 0 ; i< 8 ; i++) {
        CuAssertIntEquals(cuTest, original[i], answer[i]);
    }
}

void testHidelsb2new(CuTest *const cuTest) {
    uint8_t secret[2] = {0x3C, 0x55};
    uint8_t original[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    lsb2Hide(secret, 2, original);
    uint8_t answer[8] = {0x00, 0x03, 0x03, 0x00, 0x01, 0x01, 0x01, 0x01};
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

void testHideAndShowlsb2new(CuTest *const cuTest) {
    uint8_t secret[4] = {0x3C, 0x55, 0xA1, 0x78};
    uint8_t original[16] = {
        0x3C, 0x55, 0xA1, 0x78,
        0xE2, 0x4F, 0x19, 0xBD,
        0x6A, 0xC7, 0x8D, 0x02,
        0x51, 0xF9, 0x33, 0xAF
    };
    lsb2Hide(secret, 4, original);
    uint8_t maybeSecret[4];
    lsb2Show(original, 4, maybeSecret);
    for (int i = 0 ; i< 4 ; i++) {
        CuAssertIntEquals(cuTest, secret[i], maybeSecret[i]); 
    }
}


