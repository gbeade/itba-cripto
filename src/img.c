#include "../include/img.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "../include/shadow.h"
#include "../include/bmp.h"
#include "../include/steganography.h"

#define MAX_K 8
#define MIN_K 3


static int getBmpFilePaths(char namesArray[][256], char * folderPath) {

    DIR* directory = opendir(folderPath);
if (directory == NULL) {
        printf("Failed to open directory.\n");
        return 1;
    }
    int imagesNum = 0;

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filePath[256];
            snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, entry->d_name);

            // Check if the file is an image file based on its extension
            char* extension = strrchr(entry->d_name, '.');
            if (extension != NULL && (strcmp(extension, ".bmp") == 0)) {
                printf("Image file found: %s\n", filePath);
                strcpy(namesArray[imagesNum++], filePath);
            }
        }
    }

    closedir(directory);

    return imagesNum;
}

void recover(char* shadowPath, char* imgPath, int k) {
    uint8_t * (* lsbShow)(uint8_t * offuscated, int secretLength, uint8_t * secretOutput);

    if (k == 3 || k == 4) {
        lsbShow = lsb2Show;
    } else {
        lsbShow = lsb4Show;
    }

    char shadowImgs[MAX_K][256];
    int imagesNum = getBmpFilePaths(shadowImgs, shadowPath);

    BMPMap* bmpMap = newBmpMap(imgPath);
    BMPImage* bmpImg = mapToBmpImage(bmpMap);

    int secretLength = bmpImg->header->image_size;
    int shadowSize = secretLength / (k - 1);

    uint8_t shadows[MAX_K][shadowSize];
    int ids[MAX_K];
    uint8_t * shadowPtrs[MAX_K];

    for (int i=0 ; i<k ; i++) {
        BMPMap * shadowBmpMap = newBmpMap(shadowImgs[i]);
        BMPImage * shadowBmpImg = mapToBmpImage(shadowBmpMap);
        ids[i] = shadowBmpImg->header->reserved1;
        lsbShow(shadowBmpImg->data, shadowSize, shadows[i]);
        freeBmpImage(shadowBmpImg);
        freeBmpMap(shadowBmpMap);
        shadowPtrs[i] = shadows[i];
    }

    uint8_t * secret = reconstruct(shadowPtrs, ids, shadowSize, k);
        BMPMap * shadowBmpMap = newBmpMap(shadowImgs[0]);
        BMPImage * shadowBmpImg = mapToBmpImage(shadowBmpMap);
        BMPImage * secretImg = bytesToBmpImage(shadowBmpImg->header, secret);
        dumpBmpToFile(secretImg, imgPath);
        freeBmpImage(secretImg);
        freeBmpImage(shadowBmpImg);
        freeBmpMap(shadowBmpMap);
}

void distribute(char* shadowPath, char* imgPath, int k) {

    uint8_t * (* lsbHide)(uint8_t * secret, int secretLength, uint8_t * original);

    if (k == 3 || k == 4) {
        lsbHide = lsb2Hide;
    } else {
        lsbHide = lsb4Hide;
    }

    char shadowImgs[MAX_K][256];
    int imagesNum = getBmpFilePaths(shadowImgs, shadowPath);

    // todo: check img num

    BMPMap* bmpMap = newBmpMap(imgPath);
    BMPImage* bmpImg = mapToBmpImage(bmpMap);

    int secretLength = bmpImg->header->image_size;
    int shadowSize = secretLength / (k - 1);

    uint8_t** shadows = generateShadows(bmpImg->data, bmpImg->header->image_size, k, imagesNum);

    for (int i=0 ; i<imagesNum ; i++) {
        BMPMap * shadowBmpMap = newBmpMap(shadowImgs[0]);
        BMPImage * shadowBmpImg = mapToBmpImage(shadowBmpMap);
        shadowBmpImg->header->reserved1 = i+1;
        lsbHide(shadows[i], shadowSize, shadowBmpImg->data);
        dumpBmpToFile(shadowBmpImg, shadowImgs[0]);
        freeBmpImage(shadowBmpImg);
        freeBmpMap(shadowBmpMap);
    }

}