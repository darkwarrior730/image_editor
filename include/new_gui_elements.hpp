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

    virtual bool setX(float x);
    virtual bool setY(float y);
    virtual bool setWidth(float w);
    virtual bool setHeight(float h);

    virtual float getX();
    virtual float getY();
    virtual float getWidth();
    virtual float getHeight();

    virtual void draw();

    virtual bool checkCollide(float x, float y);
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
    Shader shader = Shader("../src/vertex/newtest3.vs", "../src/fragment/newtest3.fs");

public:
    COLORED_BOX(float x, float y, float w, float h, color c);

    virtual bool setX(float x) override;
    virtual bool setY(float y) override;
    virtual bool setWidth(float w) override;
    virtual bool setHeight(float h) override;

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
    Shader shader = Shader("../src/vertex/newtest2.vs", "../src/fragment/newtest2.fs");

public:
    TEXTURED_BOX(float x, float y, float w, float h, const char *pic, float u, float v, float uw, float vh);

    virtual bool setX(float x) override;
    virtual bool setY(float y) override;
    virtual bool setWidth(float w) override;
    virtual bool setHeight(float h) override;

    void updateVertexBuffer();

    void update_texture();

    void setVertexTexCoord (int vert, float u, float v);

    virtual void draw() override;
};

class BUTTON {
private:
    BASE_BOX *up;
    BASE_BOX *down;

    enum state {DOWN, UP} state = DOWN;
public:
    BUTTON(BASE_BOX *u, BASE_BOX *d);

    virtual void draw();

    virtual bool checkCollide(float x, float y);

    virtual int changeState();
    virtual int getState();

    virtual bool setX(float x);
    virtual bool setY(float y);
    virtual bool setWidth(float w);
    virtual bool setHeight(float h);

    virtual float getX();
    virtual float getY();
    virtual float getWidth();
    virtual float getHeight();
};