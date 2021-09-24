#pragma once

#include "gui_elements.hpp"
#include "image_handler.hpp"
#include "shader_s.h"

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color;

class drawingSpace {
public:

    /*unsigned int VBO, VAO, EBO;
    float vertices[32] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };*/
    GUI_TEXTURED_BOX gui = GUI_TEXTURED_BOX("none");

    Image *canvas;

    float zoom_scale = 1.0f;
    /*Shader testShader = Shader("../src/vertex/test2.vs", "../src/fragment/test2.fs");*/
    /*unsigned int texture;
    Image img = Image();*/

    drawingSpace();

    void createBlankCanvas(int x, int y);
    void createImageCanvas(const char *file);

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

    struct color getPixel(int x, int y);

    void draw();
};