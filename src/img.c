#include "../include/img.h"
#include "../include/bmp.h"
#include "../include/shadow.h"
#include "../include/steganography.h"

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define DT_REG 8
#define MAX_SHADOWS 10
#define MAX_FILEPATH 1000

static int mapAllBmps(char* shadowPath, BMPMap** maps) {

    DIR* dir;
    struct dirent* entry;
    struct stat fileStat;

    dir = opendir(shadowPath);
    if (dir == NULL) {
        fprintf(stderr, "Error: could not open directory.\n");
        return -1;
    }

    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        char filePath[MAX_FILEPATH];

        sprintf(filePath, "%s/%s", shadowPath, entry->d_name);
        if (stat(filePath, &fileStat) >= 0 && S_ISREG(fileStat.st_mode)) {
            maps[count++] = newBmpMap(filePath);
        }
    }

    closedir(dir);

    return count;
}

static void freeShadowImages(BMPImage** images, BMPMap** maps, int count) {
    for (int i = 0; i < count; i++) {
        if (images && images[i]) {
            freeBmpImage(images[i]);
        }
        if (maps && maps[i]) {
            freeBmpMap(maps[i]);
        }
    }
}

void distribute(char* shadowPath, char* imgPath, int k) {
    uint8_t* (*lsbHide)(uint8_t* secret, int secretLength, uint8_t* original);

    if (k >= 3 && k <= 4) {
        lsbHide = lsb4Hide;
    } else if (k <= 8) {
        lsbHide = lsb2Hide;
    } else {
        fprintf(stderr, "Error: k should be between 3 and 8\n");
        return;
    }

    // /* Load BMP for main image */
    BMPMap* mainMap = newBmpMap(imgPath);
    if(mainMap == NULL){
        return;
    }
    BMPImage* mainImage = mapToBmpImage(mainMap);
    int secretLength = mainImage->header->width * mainImage->header->height; // Might be cause of error

    if (secretLength % (2 * k - 2) != 0) {
        fprintf(stderr, "Error: image length should be multiple of 2k-2\n");
        freeBmpImage(mainImage);
        freeBmpMap(mainMap);
        return;
    }

    /* Load BMPs for shadow files */
    BMPMap* shadowMaps[MAX_SHADOWS];
    int count = mapAllBmps(shadowPath, shadowMaps);

    if(count == -1){
        freeBmpImage(mainImage);
        freeBmpMap(mainMap);
        return;
    }else if (count < k) {
        fprintf(stderr, "Error: the number of images in directory has to be at least k\n");
        shadowMaps[count++] = mainMap;
        freeShadowImages(NULL, shadowMaps, count);
        return;
    }

    BMPImage* shadowImages[MAX_SHADOWS];

    for (int i = 0; i < count; i++) {
        shadowImages[i] = mapToBmpImage(shadowMaps[i]);
    }

    for (int i = 0; i < count; i++) {
        int shadowImageLength = shadowImages[i]->header->width * shadowImages[i]->header->height;
        if (shadowImageLength != secretLength) {
            freeShadowImages(shadowImages, shadowMaps, count);
            fprintf(stderr, "Error: shadow images should be the same length of input image\n");
            return;
        }
    }

    /* Generate the shadows for the main image */
    uint8_t** byteShadows = generateShadows(mainImage->data, secretLength, k, count);

    /* For each shadow (i.e. an ary of bytes), hide it in ith image in the directory */
    for (int i = 0; i < count; i++) {
        lsbHide(byteShadows[i], secretLength / (k - 1), shadowImages[i]->data);
        labelBmpImage(shadowImages[i], i + 1);                      /* Add the ID of the shadow in the reserved byte */
        syncBmp(shadowMaps[i], shadowImages[i]->header->file_size); // Optional
    }

    /* Free resources and goodbye */
    for (int i = 0; i < count; i++) {
        freeBmpImage(shadowImages[i]);
        freeBmpMap(shadowMaps[i]);
        free(byteShadows[i]);
    }

    freeBmpImage(mainImage);
    freeBmpMap(mainMap);
    free(byteShadows);
}

void recover(char* shadowPath, char* imgPath, int k) {
    uint8_t* (*lsbShow)(uint8_t* offuscated, int secretLength, uint8_t* secretOutput);

    if (k >= 3 && k <= 4) {
        lsbShow = lsb4Show;
    } else if (k <= 8) {
        lsbShow = lsb2Show;
    } else {
        fprintf(stderr, "Error: k should be between 3 and 8\n");
        return;
    }
    /* Load BMPs for shadow files */
    BMPMap* shadowMaps[MAX_SHADOWS];
    int count = mapAllBmps(shadowPath, shadowMaps);

    if(count == -1){
        return;
    }else if (count < k) {
        fprintf(stderr, "Error: the number of images in directory has to be at least k\n");
        freeShadowImages(NULL, shadowMaps, count);
        return;
    }

    BMPImage* shadowImages[MAX_SHADOWS];

    for (int i = 0; i < count; i++) {
        shadowImages[i] = mapToBmpImage(shadowMaps[i]);
    }

    /* Take the first of all pictures as template, they are the same size */
    int secretLength = shadowImages[0]->header->width * shadowImages[0]->header->height; // Might be cause of error

    if (secretLength % (2 * k - 2) != 0) {
        fprintf(stderr, "Error: image length should be multiple of 2k-2\n");
        freeShadowImages(shadowImages, shadowMaps, count);
        return;
    }

    for (int i = 1; i < count; i++) {
        int shadowSecretLength = shadowImages[i]->header->width * shadowImages[i]->header->width;
        if (shadowSecretLength != secretLength) {
            fprintf(stderr, "Error: shadow images should be the same length\n");
            freeShadowImages(shadowImages, shadowMaps, count);
            return;
        }
    }

    uint8_t* byteShadows[MAX_SHADOWS];
    int ids[MAX_SHADOWS];
    for (int i = 0; i < k; i++) {
        byteShadows[i] = malloc(sizeof(uint8_t) * secretLength / (k - 1));
        ids[i] = shadowImages[i]->header->reserved1;
        lsbShow(shadowImages[i]->data, secretLength / (k - 1), byteShadows[i]);
    }

    /* We now have the shadows and the ids. Reconstruct the data array. */
    uint8_t* secretBytes = reconstruct(byteShadows, ids, secretLength / (k - 1), k);

    if (secretBytes == NULL) {
        fprintf(stderr, "Error: could not reconstruct image\n");
        freeShadowImages(shadowImages, shadowMaps, count);
        for (int i = 0; i < k; i++) {
            free(byteShadows[i]);
        }
        return;
    }

    BMPImage* image = bytesToBmpImage((uint8_t*)shadowImages[0]->header, secretBytes);
    dumpBmpToFile(image, imgPath);

    /* Free resources and goodbye */
    for (int i = 0; i < k; i++) {
        free(byteShadows[i]);
    }

    for (int i = 0; i < count; i++) {
        freeBmpImage(shadowImages[i]);
        freeBmpMap(shadowMaps[i]);
    }

    free(secretBytes);
    freeBmpImage(image);
}