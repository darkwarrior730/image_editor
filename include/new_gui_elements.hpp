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

typedef struct color{
    float r;
    float g;
    float b;
    float a;
} color;


class BASE_BOX {
private:
    float x_pos;
    float y_pos;
    float width;
    float height;

public:
    BASE_BOX(float x, float y, float w, float h);

    virtual void setX(float x);
    virtual void setY(float y);
    virtual void setWidth(float w);
    virtual void setHeight(float h);

    virtual float getX();
    virtual float getY();
    virtual float getWidth();
    virtual float getHeight();

    virtual void draw();
};

class COLORED_BOX : public BASE_BOX {
private:
    color col;

    unsigned int VBO, VAO, EBO;
    float vertices[24] = {
        // positions          // colors
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,  // top right
         1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,  // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f   // top left 
    };
    unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    Shader shader = Shader("../src/vertex/test3.vs", "../src/fragment/test3.fs");

public:
    COLORED_BOX(float x, float y, float w, float h, color c);

    virtual void setX(float x) override;
    virtual void setY(float y) override;
    virtual void setWidth(float w) override;
    virtual void setHeight(float h) override;

    void updateVertexBuffer();

    void setVertexColor (int vert, color c);

    virtual void draw() override;
};

class TEXTURED_BOX : public BASE_BOX {
private:
    unsigned int texture;
    Image *img;

    unsigned int VBO, VAO, EBO;
    float vertices[20] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // top right
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f   // top left 
    };
    unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    Shader shader = Shader("../src/vertex/test2.vs", "../src/fragment/test2.fs");

public:
    TEXTURED_BOX(float x, float y, float w, float h, const char *pic, float u, float v, float uw, float vh);

    virtual void setX(float x) override;
    virtual void setY(float y) override;
    virtual void setWidth(float w) override;
    virtual void setHeight(float h) override;

    void updateVertexBuffer();

    void update_texture();

    void setVertexTexCoord (int vert, float u, float v);

    virtual void draw() override;
};