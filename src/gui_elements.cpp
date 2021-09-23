#include "gui_elements.hpp"
#include "shader_s.h"

#include <bits/c++config.h>
#include <cstring>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <unordered_set>

GUI_BOX::GUI_BOX () {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    setShader("../src/vertex/test3.vs", "../src/fragment/test3.fs");
}

void GUI_BOX::draw () {
    updateVertexBuffer();

    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GUI_BOX::setShader (const char *vs, const char *fs) {
    glDeleteProgram(shader.ID);
    shader = Shader(vs, fs);
}

void GUI_BOX::setVertexPos (int vert, float x, float y, float z) {
    if (fixedSize == true) {
        return;
    }
    vertices[vert*8 + 0] = x;
    vertices[vert*8 + 1] = y;
    vertices[vert*8 + 2] = z;
}

void GUI_BOX::setVertexColor (int vert, float r, float g, float b) {
    vertices[vert*8 + 3] = r;
    vertices[vert*8 + 4] = g;
    vertices[vert*8 + 5] = b;
}

void GUI_BOX::setVertexTexCoord (int vert, float u, float v) {
    vertices[vert*8 + 6] = u;
    vertices[vert*8 + 7] = v;
}

void GUI_BOX::updateVertexBuffer () {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}

bool GUI_BOX::checkFixed(int edge) {
    typedef struct pair {
        GUI_BOX *box;
        int edge;

        bool operator==(pair const p) const {
            if ((box == p.box) && (edge == p.edge)) {
                return true;
            }
            return false;
        }
    } pair;

    struct hash_pair {
        std::size_t operator()(pair const p) const {
            return std::hash<long>()((long)p.box *(1l + (long)p.edge));
        }
    };

    std::vector<pair*> toVisit;
    toVisit.push_back(new pair{this, edge});
    std::unordered_set<pair, hash_pair> visited;
    pair* cur;
    //int a = 0;
    while (!toVisit.empty()) {
        cur = toVisit.back();
        toVisit.pop_back();
        if (visited.count(*cur) != 0) {
            continue;
        }
        visited.insert(*cur);
        if (cur->box->fixedSize == true) {
            return true;
        } else if (cur->edge%2 == 0) {
            if (cur->box->fixedY == true) {
                return true;
            } else if ((cur->edge == 0) && (cur->box->fixedTop)) {
                return true;
            } else if ((cur->edge == 2) && (cur->box->fixedBottom)) {
                return true;
            }
        } else {
            if (cur->box->fixedX == true) {
                return true;
            } else if ((cur->edge == 1) && (cur->box->fixedRight)) {
                return true;
            } else if ((cur->edge == 3) && (cur->box->fixedLeft)) {
                return true;
            }
        }
        for (int i = 0; i < cur->box->anchors[cur->edge]->pntrs.size(); i++) {
            toVisit.push_back(new pair{cur->box->anchors[cur->edge]->pntrs[i], cur->box->anchors[cur->edge]->sides[i]});
        }
    }
    return false;
}

void GUI_BOX::updateAnchors(int e, float n) {
    typedef struct pair {
        GUI_BOX* box;
        int edge;

        bool operator==(pair const p) const {
            if ((box == p.box) && (edge == p.edge)) {
                return true;
            }
            return false;
        }
    } pair;

    struct hash_pair {
        std::size_t operator()(pair const p) const {
            return std::hash<long>()((long)p.box * (long)p.edge);
        }
    };

    std::vector<pair*> toVisit;
    toVisit.push_back(new pair{this, e});
    std::unordered_set<pair, hash_pair> visited;
    pair* cur;
    while (!toVisit.empty()) {
        cur = toVisit.back();
        toVisit.pop_back();
        if (visited.count(*cur) != 0) {
            continue;
        }
        visited.insert(*cur);
        if (cur->edge == GUI_TOP) {
            cur->box->vertices[1] = n;
            cur->box->vertices[25] = n;
        } else if (cur->edge == GUI_RIGHT) {
            cur->box->vertices[0] = n;
            cur->box->vertices[8] = n;
        } else if (cur->edge == GUI_BOTTOM) {
            cur->box->vertices[9] = n;
            cur->box->vertices[17] = n;
        } else if (cur->edge == GUI_LEFT) {
            cur->box->vertices[16] = n;
            cur->box->vertices[24] = n;
        }
        cur->box->updateRel();
        for (int i = 0; i < cur->box->anchors[cur->edge]->pntrs.size(); i++) {
            toVisit.push_back(new pair{cur->box->anchors[cur->edge]->pntrs[i], cur->box->anchors[cur->edge]->sides[i]});
        }
    }

}

void GUI_BOX::setEdge (int e, float n) {
    if (checkFixed(e) == true) {
        return;
    }

    /*if (e == GUI_TOP) {
        vertices[1] = n;
        vertices[25] = n;
    } else if (e == GUI_RIGHT) {
        vertices[0] = n;
        vertices[8] = n;
    } else if (e == GUI_BOTTOM) {
        vertices[9] = n;
        vertices[17] = n;
    } else if (e == GUI_LEFT) {
        vertices[16] = n;
        vertices[24] = n;
    }*/
    
    updateAnchors(e, n);
    updateRel();
}

void GUI_BOX::setFillColor (float r, float g, float b) {
    setVertexColor(0, r, g, b);
    setVertexColor(1, r, g, b);
    setVertexColor(2, r, g, b);
    setVertexColor(3, r, g, b);
}

float GUI_BOX::getEdge (int e) {
    if (e == GUI_TOP) {
        return vertices[1];
    } else if (e == GUI_RIGHT) {
        return vertices[0];
    } else if (e == GUI_BOTTOM) {
        return vertices[9];
    } else if (e == GUI_LEFT) {
        return vertices[16];
    }
    return 0.0f;
}

float GUI_BOX::getWidth () {
    return getEdge(GUI_RIGHT) - getEdge(GUI_LEFT);
}

float GUI_BOX::getHeight () {
    return getEdge(GUI_TOP) - getEdge(GUI_BOTTOM);
}

void GUI_BOX::anchorEdge (int e1, GUI_BOX *b, int e2) {
    anchors[e1]->pntrs.push_back(b);
    anchors[e1]->sides.push_back(e2);
    b->anchors[e2]->pntrs.push_back(this);
    b->anchors[e2]->sides.push_back(e1);
}

void GUI_BOX::updateRel () {
    for (std::vector<relPos*>::iterator it = relDependent.begin(); it != relDependent.end(); ++it) {
        // move centre of each relation to its new position
        // find actual coords of rel pos
        float hoffset = getHeight() * (*it)->relY;
        float woffset = getWidth() * (*it)->relX;

        float tempx = getEdge(GUI_LEFT) + woffset;
        float tempy = getEdge(GUI_BOTTOM) + hoffset;
        
        // offset from centre of object to correct corner
        float oox = (*it)->box->getWidth() / 2.0f;
        float ooy = (*it)->box->getHeight() / 2.0f;

        float actx = tempx - oox;
        float acty = tempy - ooy;

        // move object to correct pos
        float temph = (*it)->box->getHeight();
        float tempw = (*it)->box->getWidth();

        (*it)->box->setEdge(GUI_LEFT, actx);
        (*it)->box->setEdge(GUI_BOTTOM, acty);
        (*it)->box->setEdge(GUI_RIGHT, actx+tempw);
        (*it)->box->setEdge(GUI_TOP, acty+temph);
    }
}

void GUI_BOX::addRel (GUI_BOX *b, float x, float y) {
    relDependence = {b, x, y};
    b->relDependent.push_back(new relPos{this, x, y});
    b->updateRel();
}

int GUI_BOX::checkCollide (double xpos, double ypos) {
    int i = 1;
    //std::cout << "cc : " << name << std::endl;
    //std::cout << xpos << " : " << ypos << std::endl;
    if ((xpos < getEdge(GUI_LEFT)) || (xpos > getEdge(GUI_RIGHT))) {
        i = 0;
        //std::cout << "a" << std::endl;
        //std::cout << getEdge(GUI_LEFT) << " - " << getEdge(GUI_RIGHT) << std::endl;
    }
    if ((ypos > getEdge(GUI_TOP)) || (ypos < getEdge(GUI_BOTTOM))) {
        i = 0;
        //std::cout << "b" << std::endl;
        //std::cout << getEdge(GUI_LEFT) << " - " << getEdge(GUI_RIGHT) << std::endl;
    }
    return i;
}




GUI_TEXTURED_BOX::GUI_TEXTURED_BOX (const char *pic){
    glGenTextures(1, &texture);

    if (strcmp(pic, "none") != 0) {
        img = new Image(pic);

        //img.changeImage(pic);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    box.setShader("../src/vertex/test2.vs", "../src/fragment/test2.fs");
}

void GUI_TEXTURED_BOX::draw () {
    glBindTexture(GL_TEXTURE_2D, texture);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
    //glGenerateMipmap(GL_TEXTURE_2D);

    box.draw();
}

void GUI_TEXTURED_BOX::change_image(Image *image) {
    img = image;
    update_texture();
}

void GUI_TEXTURED_BOX::update_texture() {
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}

int GUI_TEXTURED_BOX::checkCollide(double xpos, double ypos) {
    return box.checkCollide(xpos, ypos);
}




GUI_BUTTON::GUI_BUTTON (const char *up, const char *down, void (*df)(), void (*uf)()) {
    up_image = new Image(up);
    down_image = new Image(down);

    downfunc = df;
    upfunc = uf;

    box.change_image(up_image);
}

void GUI_BUTTON::click () {
    if (clicked == false) {
        box.change_image(down_image);
        clicked = true;
        downfunc();
    } else {
        box.change_image(up_image);
        clicked = false;
        upfunc();
    }
}

void GUI_BUTTON::draw () {
    box.draw();
}

int GUI_BUTTON::checkCollide(double xpos, double ypos) {
    return box.checkCollide(xpos, ypos);
}