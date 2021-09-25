#include "image_handler.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image (const char *pic) {
    if (strcmp(pic, "none") != 0) {
        pixels = stbi_load(pic, &width, &height, &bpp, 4);
    }
}

/*void Image::changeImage (const char *pic) {
    freeImage();
    pixels = stbi_load(pic, &width, &height, &bpp, 3);   
}*/

void Image::writePixels () {
    stbi_write_jpg("image.jpg", width, height, 4, pixels, 100);
}

void Image::freeImage () {
    stbi_image_free(pixels);
}