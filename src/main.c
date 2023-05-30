#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/bmp.h"
#include "../include/img.h"
#include "../include/polynomial.h"
#include "../include/shadow.h"
#include "../include/steganography.h"
#include "../include/img.h"
#include <time.h>

#define GET_INT(arr, idx) (((uintptr_t) * ((arr) + (idx))))
#define GET_CHARP(arr, idx) (((char*)*((arr) + (idx))))
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

enum {
    MODE,
    INPUT_PATH,
    OUTPUT_PATH,
    RECOVER_NUM,
    NUM_ARGUMENTS,
    DISTRIBUTE,
    RECOVER
};

char * ARG_STRINGS[] = {"[-d | -r]", "-i", "-o", "-k"};

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
        if (GET_CHARP(args, i) == NULL) {
            printf("Error: Missing argument %s\n", ARG_STRINGS[i]);
            return 1;
        }
    }

    uintptr_t mode = (int)GET_INT(args, MODE);
    int k = atoi(GET_CHARP(args, RECOVER_NUM));
    char* input = GET_CHARP(args, INPUT_PATH);
    char* output = GET_CHARP(args, OUTPUT_PATH);

    if (mode == DISTRIBUTE) {
        distribute(output, input, k);
    } else {
        recover(input, output, k);
    }

}





