#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp.h"


int main(int argc, char *argv[]) {
    BMPImage* bmp = loadBmp(argv[argc-1]);
    dumpBmpInverted(bmp); 
    freeBmp(bmp); 
    return 0;
}
