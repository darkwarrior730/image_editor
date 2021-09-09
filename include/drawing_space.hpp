#pragma once

#include "gui_elements.hpp"
#include "image_handler.hpp"
#include "shader_s.h"

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
    GUI_TEXTURED_BOX gui = GUI_TEXTURED_BOX("bg.jpg");

    float zoom_scale = 1.0f;
    /*Shader testShader = Shader("../src/vertex/test2.vs", "../src/fragment/test2.fs");*/
    /*unsigned int texture;
    Image img = Image();*/

    drawingSpace();
};