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

static int mapAllBmps(char * shadowPath, BMPMap ** maps) {

    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;

    dir = opendir(shadowPath);
    if (dir == NULL) {
        printf("Error opening directory.\n");
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

void distribute(char * shadowPath, char * imgPath, int k) {

    /* Load BMP for main image */
    BMPMap * mainMap = newBmpMap(imgPath); 
    BMPImage * mainImage = mapToBmpImage(mainMap);
    int secretLength = mainImage->header->width * mainImage->header->height;  // Might be cause of error 

    /* Load BMPs for shadow files */
    BMPMap * shadowMaps[MAX_SHADOWS];
    BMPImage * shadowImages[MAX_SHADOWS];  
    int count = mapAllBmps(shadowPath, shadowMaps); 

    for (int i=0; i<count; i++) {
        shadowImages[i] = mapToBmpImage(shadowMaps[i]); 
    }

    /* Generate the shadows for the main image */
    uint8_t ** byteShadows = generateShadows(mainImage->data, secretLength, k, count); 

    /* For each shadow (i.e. an ary of bytes), hide it in ith image in the directory */
    char fp[13] = {'b', 'i', 'n', '/', 'o', 'u', 't', ' ', '.', 'b', 'm', 'p', 0};
    for (int i=0; i<count; i++) {
        lsb4Hide(byteShadows[i], secretLength/(k-1), shadowImages[i]->data);  // TODO - change so that it chooses LSB4 or LSB2 
        BMPImage * bmp = bytesToBmpImage((uint8_t *)mainImage->header, shadowImages[i]->data);
        fp[7] = i+'0'; 
        labelBmpImage(bmp, i);  /* Add the ID of the shadow in the reserved byte */
        dumpBmpToFile(bmp, fp); /* Dump the file to an output */ // TODO: parametrize the output directory 
        freeBmpImage(bmp); 
    }

    /* Free resources and goodbye */
    for (int i=0; i<count; i++) {
        freeBmpImage(shadowImages[i]); 
        freeBmpMap(shadowMaps[i]);
        free(byteShadows[i]); 
    } 

    freeBmpImage(mainImage); 
    freeBmpMap(mainMap); 
    free(byteShadows); 
}

void recover(char * shadowPath, char * imgPath) {

    /* Load BMPs for shadow files */
    BMPMap * shadowMaps[MAX_SHADOWS];
    BMPImage * shadowImages[MAX_SHADOWS];  
    int count = mapAllBmps(shadowPath, shadowMaps);

    for (int i=0; i<count; i++) {
        shadowImages[i] = mapToBmpImage(shadowMaps[i]); 
    }

    /* Assume that the secret is shared by as many pictures as can be found */
    int k = count; 

    /* Take the first of all pictures as template, they are the same size */
    int secretLength = shadowImages[0]->header->width * shadowImages[0]->header->height;  // Might be cause of error 

    // Use LSB2 for debug
    uint8_t ** byteShadows = malloc(sizeof(uint8_t*)*k);
    int * ids = malloc(sizeof(int)*k);  
    for (int i=0; i<k; i++) {
        byteShadows[i] = malloc(sizeof(uint8_t)*secretLength/(k-1));
        ids[i] = shadowImages[i]->header->reserved1;  
        lsb4Show(shadowImages[i]->data, secretLength/(k-1), byteShadows[i]); 
    }


    /* We now have the shadows and the ids. Reconstruct the data array. */
    uint8_t * secretBytes = reconstruct(byteShadows, ids, secretLength/(k-1), k);
    
    BMPImage * image = bytesToBmpImage((uint8_t *)shadowImages[0]->header, secretBytes); 
    dumpBmpToFile(image, imgPath); 

    /* Free resources and goodbye */
    for (int i=0; i<count; i++) {
        free(byteShadows[i]); 
        freeBmpImage(shadowImages[i]); 
        freeBmpMap(shadowMaps[i]);
    } 

    free(secretBytes); 
    free(byteShadows); 
    free(ids); 
    freeBmpImage(image); 

}