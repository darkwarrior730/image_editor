#include "image_handler.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image () {
    pixels = stbi_load("bg.jpg", &width, &height, &bpp, 3);
}

void Image::writePixels () {
    stbi_write_jpg("image.jpg", width, height, 3, pixels, 100);
}

void Image::freeImage () {
    stbi_image_free(pixels);
}