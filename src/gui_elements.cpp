#include "gui_elements.hpp"
#include "shader_s.h"

#include <bits/c++config.h>
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
        for (int i = 0; i < cur->box->anchors[cur->edge]->pntrs.size(); i++) {
            toVisit.push_back(new pair{cur->box->anchors[cur->edge]->pntrs[i], cur->box->anchors[cur->edge]->sides[i]});
        }
    }

}

void GUI_BOX::setEdge (int e, float n) {
    if (checkFixed(e) == true) {
        return;
    }

    updateAnchors(e, n);
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

void GUI_BOX::anchorEdge (int e1, GUI_BOX *b, int e2) {
    anchors[e1]->pntrs.push_back(b);
    anchors[e1]->sides.push_back(e2);
    b->anchors[e2]->pntrs.push_back(this);
    b->anchors[e2]->sides.push_back(e1);
}



GUI_TEXTURED_BOX::GUI_TEXTURED_BOX () {
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void GUI_TEXTURED_BOX::draw () {
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    box.draw();
}