#pragma once

// Contains Functions For Interacting With Files
namespace Files {

    // Returns allocated buffer to data
    const char *ReadFile(const char *path);

    // Reads a BMP file and converts to RGBA + returns dimensions
    const char *ReadBMP(const char *file, int *dimensions);

}