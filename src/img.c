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

static int mapAllBmps(char * shadowPath, BMPMap ** maps, char shadowPaths[][256]) {

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
        // TODO - sacar el strcpy
        char filePath[MAX_FILEPATH];
        
        sprintf(filePath, "%s/%s", shadowPath, entry->d_name);
        if (stat(filePath, &fileStat) >= 0 && S_ISREG(fileStat.st_mode)) {
            if (shadowPaths != NULL) {
               strcpy(shadowPaths[count], filePath);
               int len = strlen(shadowPaths[count]);
               shadowPaths[count][len-4] = '2';  
               shadowPaths[count][len-3] = '.';  
               shadowPaths[count][len-2] = 'b';  
               shadowPaths[count][len-1] = 'm';  
               shadowPaths[count][len] = 'p';  
               shadowPaths[count][len+1] = '\0';  
            }
            maps[count++] = newBmpMap(filePath); 
        }
    }

    closedir(dir);

    return count; 
}

void distribute(char * shadowPath, char * imgPath, int k) {
    uint8_t * (* lsbHide)(uint8_t * secret, int secretLength, uint8_t * original);

    if (k >= 3 && k <= 4) {
        lsbHide = lsb4Hide;
    } else if (k <= 8){
        lsbHide = lsb2Hide;
    } else {
        // todo handle error
    }

    // /* Load BMP for main image */
    BMPMap * mainMap = newBmpMap(imgPath); 
    BMPImage * mainImage = mapToBmpImage(mainMap);
    int secretLength = mainImage->header->width * mainImage->header->height;  // Might be cause of error 

    if (secretLength % (2*k-2) != 0) {
        // todo handle error
    }

    /* Load BMPs for shadow files */
    BMPMap * shadowMaps[MAX_SHADOWS];
    char shadowPaths[MAX_SHADOWS][256];
    BMPImage * shadowImages[MAX_SHADOWS];  
    int count = mapAllBmps(shadowPath, shadowMaps, shadowPaths);

    if (count < k) {
        // todo handle error
    }

    for (int i=0; i<count; i++) {
        shadowImages[i] = mapToBmpImage(shadowMaps[i]); // todo should we check that image size is multiple of 2k-2?
    }

    /* Generate the shadows for the main image */
    uint8_t ** byteShadows = generateShadows(mainImage->data, secretLength, k, count); 

    /* For each shadow (i.e. an ary of bytes), hide it in ith image in the directory */
    for (int i=0; i<count; i++) {
        lsbHide(byteShadows[i], secretLength/(k-1), shadowImages[i]->data); 
        BMPImage * bmp = bytesToBmpImage((uint8_t *)mainImage->header, shadowImages[i]->data);
        labelBmpImage(shadowImages[i], i+1);  /* Add the ID of the shadow in the reserved byte */
        // dumpBmpToFile(bmp, shadowPaths[i]); /* Dump the file to an output */ 
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

void recover(char * shadowPath, char * imgPath, int k) {
    uint8_t * (* lsbShow)(uint8_t * offuscated, int secretLength, uint8_t * secretOutput);

    if (k >= 3 && k <= 4) {
        lsbShow = lsb4Show;
    } else if (k<= 8){
        lsbShow = lsb2Show;
    } else {
        // todo handle error
    }
    /* Load BMPs for shadow files */
    BMPMap * shadowMaps[MAX_SHADOWS];
    BMPImage * shadowImages[MAX_SHADOWS];  
    int count = mapAllBmps(shadowPath, shadowMaps, NULL);

    if (count < k) {
        // todo handle error
    }

    for (int i=0; i<count; i++) {
        shadowImages[i] = mapToBmpImage(shadowMaps[i]); 
        debugBmp(shadowImages[i]);
    }

    /* Take the first of all pictures as template, they are the same size */
    int secretLength = shadowImages[0]->header->width * shadowImages[0]->header->height;  // Might be cause of error 

    if (secretLength % (2*k - 2) != 0) {
        // todo handle error. Should we check for every shadow?
    }

    uint8_t ** byteShadows = malloc(sizeof(uint8_t*)*k);
    int * ids = malloc(sizeof(int)*k);  
    for (int i=0; i<k; i++) {
        byteShadows[i] = malloc(sizeof(uint8_t)*secretLength/(k-1));
        ids[i] = shadowImages[i]->header->reserved1; 
        lsbShow(shadowImages[i]->data, secretLength/(k-1), byteShadows[i]); 
    }


    /* We now have the shadows and the ids. Reconstruct the data array. */
    uint8_t * secretBytes = reconstruct(byteShadows, ids, secretLength/(k-1), k);
    
    BMPImage * image = bytesToBmpImage((uint8_t *)shadowImages[0]->header, secretBytes); 
    dumpBmpToFile(image, imgPath); 

    /* Free resources and goodbye */
    for (int i=0; i<k; i++) {
        free(byteShadows[i]); 
    }

    for (int i=0; i<count; i++) {
        freeBmpImage(shadowImages[i]); 
        freeBmpMap(shadowMaps[i]);
    } 

    free(secretBytes); 
    free(byteShadows); 
    free(ids); 
    freeBmpImage(image); 
}