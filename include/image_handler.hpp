#pragma once

#include <stdint.h>

class Image {
public:
    uint8_t* pixels;
    int width, height, bpp;
    Image();
    void writePixels();
    void freeImage();
};