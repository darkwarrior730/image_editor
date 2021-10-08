#include "new_gui_elements.hpp"
#include "shader_s.h"

#include <bits/c++config.h>
#include <cstring>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <unordered_set>

BASE_BOX::BASE_BOX(float x, float y, float w, float h) : x_pos{x}, y_pos{y}, width{w}, height{h}
{
}

bool BASE_BOX::setX(float x)
{
    x_pos = x;
    return true;
}

bool BASE_BOX::setY(float y)
{
    y_pos = y;
    return true;
}

bool BASE_BOX::setWidth(float w)
{
    width = w;
    return true;
}

bool BASE_BOX::setHeight(float h)
{
    height = h;
    return true;
}

float BASE_BOX::getX()
{
    return x_pos;
}

float BASE_BOX::getY()
{
    return y_pos;
}

float BASE_BOX::getWidth()
{
    return width;
}

float BASE_BOX::getHeight()
{
    return height;
}

void BASE_BOX::draw()
{
    return;
}

bool BASE_BOX::checkCollide(float x, float y)
{
    bool b = true;
    if (x < x_pos || x > x_pos+width) {
        b = false;
    }
    if (y > y_pos || y < y_pos-height) {
        b = false;
    }
    return b;
}


COLORED_BOX::COLORED_BOX(float x, float y, float w, float h, color c) : BASE_BOX{x, y, w, h}, col{c}
{
    setVertexColor(0, col);
    setVertexColor(1, col);
    setVertexColor(2, col);
    setVertexColor(3, col);

    setX(x);
    setY(y);
    setWidth(w);
    setHeight(h);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*for (int i = 0; i < 24; ++i) {
        std::cout << vertices[i] << std::endl;
    }*/
}

bool COLORED_BOX::setX(float x)
{
    if (BASE_BOX::setX(x) == true) {
        vertices[12] = x;
        vertices[18] = x;

        if (setWidth(BASE_BOX::getWidth()) == true) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool COLORED_BOX::setY(float y)
{
    if (BASE_BOX::setY(y) == true) {
        vertices[1] = y;
        vertices[19] = y;

        if (setHeight(BASE_BOX::getHeight()) == true) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool COLORED_BOX::setWidth(float w)
{
    if (BASE_BOX::setWidth(w) == true) {
        vertices[0] = BASE_BOX::getX() + w;
        vertices[6] = BASE_BOX::getX() + w;
        return true;
    } else {
        return false;
    }
}

bool COLORED_BOX::setHeight(float h)
{
    if (BASE_BOX::setHeight(h) == true) {
        vertices[7] = BASE_BOX::getY() - h;
        vertices[13] = BASE_BOX::getY() - h;
        return true;
    } else {
        return false;
    }
}

void COLORED_BOX::updateVertexBuffer () {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}

void COLORED_BOX::draw()
{
    updateVertexBuffer();

    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    return;
}

void COLORED_BOX::setVertexColor (int vert, color c) {
    vertices[vert*6 + 3] = c.r;
    vertices[vert*6 + 4] = c.g;
    vertices[vert*6 + 5] = c.b;
}


TEXTURED_BOX::TEXTURED_BOX(float x, float y, float w, float h, const char *pic, float u, float v, float uw, float vh) : BASE_BOX{x, y, w, h}
{
    img = new Image(pic);

    setVertexTexCoord(0, u+uw, v);
    setVertexTexCoord(1, u+uw, v-vh);
    setVertexTexCoord(2, u, v-vh);
    setVertexTexCoord(3, u, v);

    setX(x);
    setY(y);
    setWidth(w);
    setHeight(h);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*for (int i = 0; i < 24; ++i) {
        std::cout << vertices[i] << std::endl;
    }*/

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}

bool TEXTURED_BOX::setX(float x)
{
    if (BASE_BOX::setX(x) == true) {
        vertices[10] = x;
        vertices[15] = x;

        if (setWidth(BASE_BOX::getWidth()) == true) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool TEXTURED_BOX::setY(float y)
{
    if (BASE_BOX::setY(y) == true) {
        vertices[1] = y;
        vertices[16] = y;

        if (setHeight(BASE_BOX::getHeight()) == true) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool TEXTURED_BOX::setWidth(float w)
{
    if (BASE_BOX::setWidth(w) == true) {
        vertices[0] = BASE_BOX::getX() + w;
        vertices[5] = BASE_BOX::getX() + w;
        return true;
    } else {
        return false;
    }
}

bool TEXTURED_BOX::setHeight(float h)
{
    if (BASE_BOX::setHeight(h) == true) {
        vertices[6] = BASE_BOX::getY() - h;
        vertices[11] = BASE_BOX::getY() - h;
        return true;
    } else {
        return false;
    }
}

void TEXTURED_BOX::updateVertexBuffer () {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}

void TEXTURED_BOX::update_texture() {
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void TEXTURED_BOX::draw()
{
    updateVertexBuffer();

    glBindTexture(GL_TEXTURE_2D, texture);

    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    return;
}

void TEXTURED_BOX::setVertexTexCoord(int vert, float u, float v) {
    vertices[vert*5 + 3] = u;
    vertices[vert*5 + 4] = v;
}


BUTTON::BUTTON(BASE_BOX *u, BASE_BOX *d) : up{u}, down{d} 
{
}

void BUTTON::draw()
{
    if (state == DOWN) {
        down->draw();
    } else if (state == UP) {
        up->draw();
    }
}

bool BUTTON::checkCollide(float x, float y)
{
    return down->checkCollide(x, y);
}

int BUTTON::changeState()
{
    if (state == DOWN) {
        state = UP;
    } else if (state == UP) {
        state = DOWN;
    }
    return state;   
}

int BUTTON::getState()
{
    return state;
}

bool BUTTON::setX(float x)
{
    if (down->setX(x) == true && up->setX(x) == true) {
        return true;
    } else {
        return false;
    }
}

bool BUTTON::setY(float y)
{
    if (down->setY(y) == true && up->setY(y) == true) {
        return true;
    } else {
        return false;
    }   
}

bool BUTTON::setWidth(float w)
{
    if (down->setWidth(w) == true && up->setWidth(w) == true) {
        return true;
    } else {
        return false;
    }
}

bool BUTTON::setHeight(float h)
{
    if (down->setHeight(h) == true && up->setHeight(h) == true) {
        return true;
    } else {
        return false;
    }
}

float BUTTON::getX()
{
    return down->getX();
}

float BUTTON::getY()
{
    return down->getY();
}

float BUTTON::getWidth()
{
    return down->getWidth();
}

float BUTTON::getHeight()
{
    return down->getHeight();
}