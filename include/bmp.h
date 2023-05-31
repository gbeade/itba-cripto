#ifndef BMP_H
#define BMP_H

#include <stdint.h>

typedef struct  __attribute__((__packed__)) BMPHeader {
    uint16_t magic_number;          // magic number identifying the file type (must be 0x4D42 for BMP files)
    uint32_t file_size;             // size of the BMP file in bytes
    uint16_t reserved1;             // reserved; must be set to 0
    uint16_t reserved2;             // reserved; must be set to 0
    uint32_t data_offset;           // offset, in bytes, from the beginning of the BMP header to the bitmap data
    uint32_t header_size;           // size of the BMP header in bytes
    uint32_t width;                 // width of the image, in pixels
    uint32_t height;                // height of the image, in pixels
    uint16_t planes;                // number of planes in the image; must be set to 1
    uint16_t bits_per_pixel;        // number of bits per pixel (must be 8, 16, 24, or 32)
    uint32_t compression_method;    // compression method used for the image (0 = none, 1 = RLE-8, 2 = RLE-4)
    uint32_t image_size;            // size of the image data in bytes (can be set to 0 if compression is not used)
    uint32_t x_pixels_per_meter;    // horizontal resolution of the image, in pixels per meter
    uint32_t y_pixels_per_meter;    // vertical resolution of the image, in pixels per meter
    uint32_t num_colors;            // number of colors in the color palette (0 = use maximum allowed by bits per pixel)
    uint32_t important_colors;      // number of important colors in the color palette (0 = all are important)
} BMPHeader;

typedef struct BMPMap {
    uint8_t * map;  // Memory-mapped array of bytes that represent a .bmp
    int fd; 
} BMPMap; 

typedef struct BMPImage {
    BMPHeader * header; 
    uint8_t * data;  // 0x00->black, 0xFF->white
} BMPImage;


/* BMP LOADING AND UNLOADING */

/* Memory-maps a file and returns a struct with information about its memory location*/
BMPMap * newBmpMap(const char * path); 

/* Interprets a memory-mapped bmp file as a BMPImage structure. */
BMPImage * mapToBmpImage(BMPMap * map); 

/* Interprets a arrays of bytes BMPImage structure. */
BMPImage * bytesToBmpImage(uint8_t * header, uint8_t * data); 

/* Releases the resources used by the BMPImage structure */
void freeBmpImage(BMPImage * bmpImage);

/* Releases the resources used by the BMPMap structure */
void freeBmpMap(BMPMap * bmpMap);


/* Creates a deep copy of a BMP header and returns a pointer to the resulting BMPHeader. */
BMPHeader* cloneBmpHeader(BMPHeader* src); 

/* Dumps all bytes of a BMPImage into a file in specified path. If it doesn't exist, the file is created. */
void dumpBmpToFile(BMPImage * bmp, char * path);

/* Dumps all bytes of a BMPImage into stdout. It can be piped into a file. */
void dumpBmp(BMPImage* bmp); 

/* Syncs the first n bytes of a BMPMap to disk */
void syncBmp(BMPMap * bmpMap, int size); 



/* BMP MANIPULATION */

/* Sets the reserved1 byte of the header to the shadow label value */
void labelBmpImage(BMPImage * bmp, uint16_t label);

/* Dumps all bytes of a BMPImage into stdout in an inverted format. It can be piped into a file. */
void dumpBmpInverted(BMPImage* bmp);

/* Prints debugging information about the BMPImage, including its header and image data. */
void debugBmp(BMPImage* bmp);  


#endif // BMP_H
