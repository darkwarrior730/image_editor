#pragma once

#include "shader_s.h"
#include "image_handler.hpp"

#include <vector>

#define GUI_TOP 0
#define GUI_RIGHT 1
#define GUI_BOTTOM 2
#define GUI_LEFT 3

class GUI_BOX;

typedef struct anchorSide{
    std::vector<GUI_BOX*> pntrs;
    std::vector<int> sides;
} anchorSide;

class GUI_BOX {
public:
    unsigned int VBO, VAO, EBO;
    float vertices[32] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
    //    -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    //    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    //    1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    //    1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f
    };
    unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    Shader shader = Shader("../src/vertex/test2.vs", "../src/fragment/test2.fs");

    bool fixedSize = false;

    bool fixedX = false;
    bool fixedY = false;

    bool fixedTop = false;
    bool fixedRight = false;
    bool fixedBottom = false;
    bool fixedLeft = false;

    bool ancTop = false;
    GUI_BOX* anchorTop;
    int nextTop;

    anchorSide topSide;

    bool ancRight = false;
    GUI_BOX* anchorRight;
    int nextRight;

    anchorSide rightSide;

    bool ancBottom = false;
    GUI_BOX* anchorBottom;
    int nextBottom;

    anchorSide bottomSide;

    bool ancLeft = false;
    GUI_BOX* anchorLeft;
    int nextLeft;

    anchorSide leftSide;

    anchorSide* anchors[4] = {&topSide, &rightSide, &bottomSide, &leftSide};

    GUI_BOX();

    void draw();

    void setShader(const char* vs, const char* fs);

    void setVertexPos(int vert, float x, float y, float z);
    void setVertexColor(int vert, float r, float g, float b);
    void setVertexTexCoord(int vert, float u, float v);

    void updateVertexBuffer();

    void setEdge(int e, float n);
    float getEdge(int e);

    void setFillColor(float r, float g, float b);

    void anchorEdge(int e1, GUI_BOX* b, int e2);

    //void checkAnchors(int e, float val);

    bool checkFixed(int edge);

    void updateAnchors(int e, float n);
};

class GUI_TEXTURED_BOX {
public:
    GUI_BOX box = GUI_BOX();
    unsigned int texture;
    Image img = Image();

    GUI_TEXTURED_BOX();

    void draw();
};

/*
basic color fill shader for GUI_BOX   test3
texture shader for GUI_TEXTURED_BOX   test2
*/