#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/bmp.h"
#include "../include/img.h"
#include "../include/polynomial.h"
#include "../include/shadow.h"
#include "../include/steganography.h"

void printBits(void const* const ptr, size_t const size) {
    unsigned char* b = (unsigned char*)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
}

void printBytes(uint8_t* vec, int bytes) {
    for (int i = 0; i < bytes; i++) {
        printBits(vec + i, 1);
    }
    puts("");
}

void tryBmp(char* path) {
    BMPMap* bmpMap = newBmpMap(path);
    BMPImage* bmpImage = mapToBmpImage(bmpMap);

    // BMPHeader * bmpHeaderNew = bmpImage->header;

    BMPHeader* bmpHeaderNew = cloneBmpHeader(bmpImage->header); // ERROR IN THIS FUNCTION!!
    BMPImage* bmp2 = bytesToBmpImage((uint8_t*)bmpHeaderNew, bmpImage->data);
    dumpBmpToFile(bmp2, "bin/out.bmp");
    free(bmpHeaderNew);

    freeBmpImage(bmpImage);
    freeBmpMap(bmpMap);
    freeBmpImage(bmp2);
}

void tryPolynomial() {
    printf("\nBeginning test poly\n");

    // Create polynomial from coefficients
    Polynomial* poly = polyFromCoefficients(3, 1, -1, 1);

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
    Polynomial* poly = polyInterpolate(N, xs, ys);

    // Print polynomial
    polyPrint(poly);

    for (int i = 0; i < N; i++) {
        int t = polyEvaluate(poly, xs[i]);
        printf("p(%d)=%d\n", xs[i], t);
    }

    polyFree(poly);
}

void tryShadowGeneration() {

    printf("\nBeginning test shadow generation\n------\n");

    uint8_t* secret = (uint8_t*)"GONZALO";

    // Declare parameters of the schema
    int n = 5;
    int k = 3;

    // Considers the NULL-terminated
    int secretLength;
    for (secretLength = 1; secret[secretLength - 1]; secretLength++)
        ;

    int shadowSize = secretLength / (k - 1);
    uint8_t originals[n][secretLength];

    int ids[3] = {1, 2, 3};
    uint8_t** shadows = generateShadows(secret, secretLength, k, n);

    for (int i = 0; i < n; i++) {
        lsb4Hide(shadows[i], shadowSize, originals[i]);
        lsb4Show(originals[i], shadowSize, shadows[i]);
    }

    uint8_t* secretReconstructed = reconstruct(shadows, ids, shadowSize, k);

    printf("The schema is (%d, %d).\n", k, n);
    printf("The secret %s\n\n", secret);

    printf("%d shadows were created. They are:\n", n);
    for (int i = 0; i < n; i++) {
        printf("S%d -> ", i + 1);
        for (int j = 0; j < shadowSize; j++) {
            printf("0x%2X ", shadows[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');

    printf("We will now combine the first %d shadows.\n", k);
    printf("The reconstructed secret is: %s\n", secretReconstructed);
}

void tryBmpShadow(char* path) {
    BMPMap* bmpMap = newBmpMap(path);
    BMPImage* bmp = mapToBmpImage(bmpMap);

    printf("\nBeginning test bmp+shadow generation\n------\n");

    uint8_t* secret = bmp->data;

    // Declare parameters of the schema
    int n = 5;
    int k = 3;

    // Considers the NULL-terminated
    int secretLength = 512 * 512;

    int shadowSize = secretLength / (k - 1);

    int ids[3] = {1, 2, 3};
    uint8_t** shadows = generateShadows(secret, secretLength, k, n);
    uint8_t* secretReconstructed = reconstruct(shadows, ids, shadowSize, k);

    printf("The schema is (%d, %d).\n", k, n);
    printf("The secret is the bmp at %s\n\n", path);

    printf("%d shadows were created. They are:\n", n);
    for (int i = 0; i < n; i++) {
        printf("S%d -> ", i + 1);
        for (int j = 0; j < 10; j++) {
            printf("0x%2X ", shadows[i][j]);
        }
        printf("...\n");
    }
    putchar('\n');

    int t = 10; // 512*512;
    printf("We will now combine the first %d shadows.\n", k);
    printf("Do the first %d bytes match?\n", t);

    for (int i = 0; i < t; i++)
        printf("[0x%2X] ~ [0x%2X] %4s\n", secret[i], secretReconstructed[i], secret[i] == secretReconstructed[i] ? "OK" : "FAIL");

    for (int i = 0; i < n; i++)
        free(shadows[i]);
    free(shadows);
    free(secretReconstructed);
    freeBmpImage(bmp);
    freeBmpMap(bmpMap);
}
#define GET_INT(arr, idx) (((uintptr_t) * ((arr) + (idx))))
#define GET_CHARP(arr, idx) (((char*)*((arr) + (idx))))
#define INT2VOIDP(i) (void*)(uintptr_t)(i)
enum {
    MODE,
    INPUT_PATH,
    OUTPUT_PATH,
    RECOVER_NUM,
    DISTRIBUTE_NUM,
    NUM_ARGUMENTS,
    DISTRIBUTE,
    RECOVER
};

char* ARG_STRINGS[] = {"[-d | -r]", "-i", "-o", "-k", "-n"};
int main(int argc, char* argv[]) {

    void* args[NUM_ARGUMENTS] = {NULL};
    int i;

    // print help if no arguments are provided or if the first argument is -h
    if (argc < 2 || strcmp("-h", argv[1]) == 0) {
        fprintf(stderr,
                "usage: %s [-d | -r] [options]...\n"
                "\n"
                "   -h                                     prints help and finish.\n"
                "   -d                                     distribute a secret image throughout a set of images.\n"
                "   -r                                     recover a secret image from a set of images.\n"
                "   -n [number]                            specify the number of images to distribute the secret among.\n"
                "   -k [number]                            specify the number of images needed to recover the secret image.\n"
                "   -i [path]                              specify input path for secret image (a directory when using -r, a file when using -d).\n"
                "   -o [path]                              specify output path for secret image (a file when using -r, a directory when using -d).\n"
                "\n",
                argv[0]);
        return 1;
    }

    // parse the arguments
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            if (args[MODE]) {
                printf("Error: argument cannot be -d and -r\n");
                return 1;
            }
            args[MODE] = INT2VOIDP(DISTRIBUTE);
        } else if (strcmp(argv[i], "-r") == 0) {
            if (args[MODE]) {
                printf("Error: argument cannot be -d and -r\n");
                return 1;
            }
            args[MODE] = INT2VOIDP(RECOVER);
        } else { // argument has a value that follows
            if (i == argc - 1) {
                printf("Error: Invalid number of arguments\n");
                return 1;
            }
            int found = 0;
            for (unsigned long j = 1; j < sizeof(ARG_STRINGS) / sizeof(ARG_STRINGS[0]); j++) {
                if (strcmp(argv[i], ARG_STRINGS[j]) == 0) {
                    args[j] = argv[i + 1];
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Error: Unknown argument %s\n", argv[i]);
                return 1;
            }
            i++;
        }
    }

    // Check that all arguments were provided
    for (i = 0; i < NUM_ARGUMENTS; i++) {
        if (GET_CHARP(args, i) == NULL && !(i == DISTRIBUTE_NUM && GET_INT(args, MODE) == RECOVER)) {
            printf("Error: Missing argument %s\n", ARG_STRINGS[i]);
            return 1;
        }
    }

    // Print out the parsed arguments
    printf("Mode: %lu\n", GET_INT(args, MODE));
    printf("Input File: %s\n", GET_CHARP(args, INPUT_PATH));
    printf("K: %s\n", GET_CHARP(args, RECOVER_NUM));
    printf("N: %s\n", GET_CHARP(args, DISTRIBUTE_NUM));
    printf("Output path: %s\n", GET_CHARP(args, OUTPUT_PATH));

    uintptr_t mode = (int)GET_INT(args, MODE);
    int k = atoi(GET_CHARP(args, RECOVER_NUM));
    char* input = GET_CHARP(args, INPUT_PATH);
    char* output = GET_CHARP(args, OUTPUT_PATH);

    if (mode == DISTRIBUTE) {
        int n = atoi(GET_CHARP(args, DISTRIBUTE_NUM));
        distribute(output, input, n, k);
    } else {
        recover(input, output, k);
    }
    return 0;
}
