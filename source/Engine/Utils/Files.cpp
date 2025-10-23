#include "Engine/Utils/Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *Files::ReadFile(const char *path) {
    FILE *fptr;
    fptr = fopen(path, "r");

    fseek(fptr, 0, SEEK_END);
    int size = ftell(fptr);
    rewind(fptr);

    char *data = (char *)calloc(size + 1, 1);
    fread(data, 1, size, fptr);

    fclose(fptr);
    return data;
}

const char *Files::ReadBMP(const char *file, int *dimensions) {
    const char *contents = Files::ReadFile(file);

    int width, height;
    memcpy(&width, contents + 0x12, 4);
    memcpy(&height, contents + 0x16, 4);

    int srcBytesPerPixel = 3;
    int dstBytesPerPixel = 4;
    int imageSize = width * height * srcBytesPerPixel;

    char *src = (char *)(contents + 0x36);
    char *rgba = (char *)malloc(width * height * dstBytesPerPixel);

    // Convert from BGR → RGBA
    for (int i = 0, j = 0; i < imageSize; i += 3, j += 4) {
        rgba[j + 0] = src[i + 2];
        rgba[j + 1] = src[i + 1];
        rgba[j + 2] = src[i + 0];
        rgba[j + 3] = 255;
    }

    dimensions[0] = width;
    dimensions[1] = height;
    return (const char *)rgba;
}