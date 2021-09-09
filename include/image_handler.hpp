#pragma once

#include <stdint.h>

class Image {
public:
    uint8_t *pixels;
    int width, height, bpp;
    Image(const char *pic);
    //void changeImage(const char *pic);
    void writePixels();
    void freeImage();
};