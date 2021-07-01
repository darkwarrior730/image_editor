#pragma once

#include <stdint.h>

class Image {
public:
    uint8_t* rgb_image;
    int width, height, bpp;
    Image();
    void writePixels();
    void freeImage();
};