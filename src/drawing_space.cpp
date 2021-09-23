#include "image_handler.hpp"
#include "shader_s.h"
#include "drawing_space.hpp"

#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

drawingSpace::drawingSpace()
{
}

void drawingSpace::createImageCanvas(const char *file) {
    canvas = new Image(file);
    gui.change_image(canvas);
}

void drawingSpace::createBlankCanvas(int x, int y) {
    canvas = new Image("none");
    canvas->bpp = 3;
    canvas->height = y;
    canvas->width = x;
    canvas->pixels = (uint8_t*)malloc(x*y*3);
    memset(canvas->pixels, 255, x*y*3);
    gui.change_image(canvas);
}

void drawingSpace::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    int temp = ((y*canvas->width + x)*3);
    *(canvas->pixels + temp) = r;
    *(canvas->pixels + temp + 1) = g;
    *(canvas->pixels + temp + 2) = b;
}

void drawingSpace::draw() {
    gui.update_texture();
    gui.draw();
}