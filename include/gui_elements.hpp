#pragma once

#include "shader_s.h"
#include "image_handler.hpp"

class GUI_BOX {
public:
    unsigned int VBO, VAO, EBO;
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
    };
    Shader shader = Shader("../src/vertex/test2.vs", "../src/fragment/test2.fs");

    GUI_BOX();

    void draw();

    void setShader(const char* vs, const char* fs);
};

class GUI_TEXTURED_BOX {
public:
    GUI_BOX box = GUI_BOX();
    unsigned int texture;
    Image img = Image();

    GUI_TEXTURED_BOX();

    void draw();
};