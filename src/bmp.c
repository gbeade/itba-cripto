#include "../include/bmp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>


BMPHeader* cloneBmpHeader(BMPHeader* src) {
    BMPHeader* dest = malloc(src->data_offset);
    memcpy(dest, src, sizeof(BMPHeader));
    return dest;
}

/* Sets the reserved1 byte of the header to the shadow label value */
void labelBmpImage(BMPImage * bmp, uint16_t label){
    bmp->header->reserved1 = label; 
}

BMPMap * newBmpMap(const char* path) {

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }
    
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        perror("fstat");
        close(fd);
        return NULL;
    }

    uint8_t * map = (uint8_t*) mmap(NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return NULL;
    }

    BMPHeader* bmpHeader = (BMPHeader*) map;
    if (bmpHeader->magic_number != 0x4D42) {
        fprintf(stderr, "Invalid BMP magic number\n");
        munmap(map, file_stat.st_size);
        close(fd);
        return NULL;
    }

    if (bmpHeader->bits_per_pixel != 8) {
        fprintf(stderr, "Only 8-bit BMP images are supported. This file is was %d\n", bmpHeader->bits_per_pixel);
        munmap(map, file_stat.st_size);
        close(fd);
        return NULL;
    }

    BMPMap * bmpMap = malloc(sizeof(BMPMap));
    bmpMap->map = map; 
    bmpMap->fd = fd; 
    return bmpMap;
}

BMPImage * mapToBmpImage(BMPMap * bmpMap) {
    BMPImage * bmpImage = (BMPImage *) malloc(sizeof(BMPImage)); 
    bmpImage->header = (BMPHeader *)bmpMap->map; 
    uint8_t* data = bmpMap->map+bmpImage->header->data_offset;
    bmpImage->data = data; 
    return bmpImage; 
}

BMPImage * bytesToBmpImage(uint8_t * header, uint8_t * data) {
    BMPImage * bmpImage = (BMPImage *) malloc(sizeof(BMPImage)); 
    bmpImage->header = (BMPHeader *)header; 
    bmpImage->data = data; 
    return bmpImage; 
}


void dumpBmpInverted(BMPImage* bmp) {
    // Dump header as-is
    fwrite(bmp->header, bmp->header->data_offset, 1, stdout);

    // Dump inverted image data
    int width = bmp->header->width;
    int height = bmp->header->height;
    int padding = (4 - (width % 4)) % 4;  // BMP row padding is rounded up to a multiple of 4 bytes
    uint8_t* data = bmp->data;

    for (int y = height - 1; y >= 0; y--) {  // iterate over rows in reverse order
        for (int x = 0; x < width; x++) {   // iterate over pixels in row
            uint8_t pixel = *(data + y*(width + padding) + x);
            uint8_t inverted_pixel = ~pixel; // invert bits of pixel
            fwrite(&inverted_pixel, sizeof(uint8_t), 1, stdout);
        }
        // write row padding
        uint8_t padding_byte = 0x00;
        for (int i = 0; i < padding; i++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, stdout);
        }
    }
}

/* Dumps all bytes of a BMPImage into stdout. It can be piped into a file. */
void dumpBmp(BMPImage* bmp){
    // Dump header as-is
    fwrite(bmp->header, bmp->header->data_offset, 1, stdout);

    // Dump inverted image data
    int width = bmp->header->width;
    int height = bmp->header->height;
    int padding = (4 - (width % 4)) % 4;  // BMP row padding is rounded up to a multiple of 4 bytes
    uint8_t* data = bmp->data;

    for (int y = height - 1; y >= 0; y--) {  // iterate over rows in reverse order
        for (int x = 0; x < width; x++) {   // iterate over pixels in row
            uint8_t pixel = *(data + y*(width + padding) + x);
            fwrite(&pixel, sizeof(uint8_t), 1, stdout);
        }
        // write row padding
        uint8_t padding_byte = 0x00;
        for (int i = 0; i < padding; i++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, stdout);
        }
    }
}

/* Dumps all bytes of a BMPImage into a file in specified path. If it doesn't exist, the file is created. */
void dumpBmpToFile(BMPImage * bmp, char * path){

    printf("%s\n", path); 
    // Open file for writing
    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        printf("ERROR\n"); 
        fprintf(stderr, "Error: could not open file for writing\n");
        return;
    }

    // Dump header as-is
    fwrite(bmp->header, bmp->header->data_offset, 1, file);

    // Dump image data
    int width = bmp->header->width;
    int height = bmp->header->height;
    int padding = (4 - (width % 4)) % 4;  // BMP row padding is rounded up to a multiple of 4 bytes
    uint8_t* data = bmp->data;

    for (int y = 0; y < height; y++) { 
        for (int x = 0; x < width; x++) {   // iterate over pixels in row
            uint8_t pixel = *(data + y*(width + padding) + x);
            fwrite(&pixel, sizeof(uint8_t), 1, file);
        }
        // write row padding
        uint8_t padding_byte = 0x00;
        for (int i = 0; i < padding; i++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, file);
        }
    }

    // Close file
    fclose(file);
}

void freeBmpImage(BMPImage * bmpImage) {
    free(bmpImage); 
}

void freeBmpMap(BMPMap * bmpMap) {
    munmap(bmpMap->map, ((BMPHeader *)bmpMap->map)->file_size);
    close(bmpMap->fd); 
    free(bmpMap);
}

void debugBmp(BMPImage * bmp) {
    printf("Image dimensions: %dx%d\n", bmp->header->width, bmp->header->height);
    printf("Bits per pixel: %d\n", bmp->header->bits_per_pixel);
    printf("Data offset: %d\n", bmp->header->data_offset);
    printf("File size: %d\n", bmp->header->file_size);
    printf("Header size: %d\n", bmp->header->header_size);
    printf("Planes: %d\n", bmp->header->planes);
    printf("Compression method: %d\n", bmp->header->compression_method);
    printf("r1: %d\n", bmp->header->reserved1);
}
