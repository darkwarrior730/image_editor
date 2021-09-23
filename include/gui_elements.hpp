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

typedef struct relPos{
    GUI_BOX *box;
    float relX;
    float relY;
} relPos;

/*
in containing box when you change size size call updateRel method of all dependent boxes
updateRel updates the boxes position with respect to the containing boxes position and size
*/

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

    anchorSide topSide;

    anchorSide rightSide;

    anchorSide bottomSide;

    anchorSide leftSide;

    anchorSide *anchors[4] = {&topSide, &rightSide, &bottomSide, &leftSide};

    std::vector<relPos*> relDependent;
    relPos relDependence;

    char *name;

    GUI_BOX();

    void draw();

    void setShader(const char *vs, const char *fs);

    void setVertexPos(int vert, float x, float y, float z);
    void setVertexColor(int vert, float r, float g, float b);
    void setVertexTexCoord(int vert, float u, float v);

    void updateVertexBuffer();

    void setEdge(int e, float n);
    float getEdge(int e);

    float getWidth();
    float getHeight();

    void setFillColor(float r, float g, float b);

    void anchorEdge(int e1, GUI_BOX *b, int e2);

    bool checkFixed(int edge);

    void updateAnchors(int e, float n);

    void updateRel();

    void addRel(GUI_BOX *b, float x, float y);

    int checkCollide(double xpos, double ypos);
};

class GUI_TEXTURED_BOX {
public:
    GUI_BOX box = GUI_BOX();
    unsigned int texture;
    Image *img;// = Image();

    char *&name = box.name;

    GUI_TEXTURED_BOX(const char *pic);

    void draw();
    void change_image(Image *image);
    void update_texture();

    int checkCollide(double xpos, double ypos);
};

/*
basic color fill shader for GUI_BOX   test3
texture shader for GUI_TEXTURED_BOX   test2
*/

class GUI_BUTTON {
public:
    GUI_TEXTURED_BOX box = GUI_TEXTURED_BOX("none");

    Image *up_image;
    Image *down_image;

    void (*downfunc)();
    void (*upfunc)();

    bool clicked = false;

    char *&name = box.name;

    GUI_BUTTON(const char *up, const char *down, void (*df)(), void (*uf)());

    void click();
    void draw();

    int checkCollide(double xpos, double ypos);
};