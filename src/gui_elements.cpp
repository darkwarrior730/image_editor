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

void GUI_BOX::setShader (const char* vs, const char* fs) {
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
        std::size_t operator()(const pair p) const {
            return std::hash<long>()((long)p.box * (long)p.edge);
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
        //std::cout << a << std::endl;
        //a++;
        //std::cout << cur->box << "#" << cur->edge << std::endl;
        //std::cout << ":::" << visited.count(*cur) << std::endl;
        //std::cout << cur << std::endl;
        if (visited.count(*cur) != 0) {
            //std::cout << "z" << std::endl;
            continue;
        }
        visited.insert(*cur);
        if (cur->box->fixedSize == true) {
            //std::cout << "a" << std::endl;
            return true;
        } else if (cur->edge%2 == 0) {
            if (cur->box->fixedY == true) {
                //std::cout << "b" << std::endl;
                return true;
            } else if ((cur->edge == 0) && (cur->box->fixedTop)) {
                //std::cout << "c" << std::endl;
                return true;
            } else if ((cur->edge == 2) && (cur->box->fixedBottom)) {
                //std::cout << "d" << std::endl;
                return true;
            }
        } else {
            if (cur->box->fixedX == true) {
                //std::cout << "e" << std::endl;
                return true;
            } else if ((cur->edge == 1) && (cur->box->fixedRight)) {
                //std::cout << "f" << std::endl;
                return true;
            } else if ((cur->edge == 3) && (cur->box->fixedLeft)) {
                //std::cout << "g" << std::endl;
                return true;
            }
        }
        for (int i = 0; i < cur->box->anchors[cur->edge]->pntrs.size(); i++) {
            toVisit.push_back(new pair{cur->box->anchors[cur->edge]->pntrs[i], cur->box->anchors[cur->edge]->sides[i]});
            //std::cout << "--" << cur->box->anchors[cur->edge]->pntrs[i] << std::endl;
            //std::cout << "--" << cur->box->anchors[cur->edge]->sides[i] << std::endl;
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
        std::size_t operator()(const pair p) const {
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
    /*if (fixedSize == true) {
        return;
    }
    if ((fixedX == true) && (e%2 == 1)) {
        return;
    }
    if ((fixedY == true) && (e%2 == 0)) {
        return;
    }
    if ((fixedTop == true) && (e == 0)) {
        return;
    }
    if ((fixedRight == true) && (e == 1)) {
        return;
    }
    if ((fixedBottom == true) && (e == 2)) {
        return;
    }
    if ((fixedLeft == true) && (e == 3)) {
        return;
    }*/
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
    }
    checkAnchors(e, n);*/
    updateAnchors(e, n);
}

/*void GUI_BOX::checkAnchors (int e, float val) {
    if (e == 0) {
        if (ancTop != true) {
            return;
        }
        if ((anchorTop->fixedSize == true) || (anchorTop->fixedY == true)) {
            vertices[1] = anchorTop->getEdge(nextTop);
            vertices[25] = anchorTop->getEdge(nextTop);
            return;
        }
        if (nextTop == 0) {
            if (anchorTop->fixedTop == true) {
                vertices[1] = anchorTop->getEdge(0);
                vertices[25] = anchorTop->getEdge(0);
                return;
            }
            anchorTop->vertices[1] = val;
            anchorTop->vertices[25] = val;
            return;
        } else if (nextTop == 2) {
            if (anchorTop->fixedBottom == true) {
                vertices[1] = anchorTop->getEdge(2);
                vertices[25] = anchorTop->getEdge(2);
                return;
            }
            anchorTop->vertices[9] = val;
            anchorTop->vertices[17] = val;
            return;
        }
    } else
    if (e == 1) {
        if (ancRight != true) {
            return;
        }
        if ((anchorRight->fixedSize == true) || (anchorRight->fixedX == true)) {
            vertices[0] = anchorRight->getEdge(nextRight);
            vertices[8] = anchorRight->getEdge(nextRight);
            return;
        }
        if (nextRight == 1) {
            if (anchorRight->fixedRight == true) {
                vertices[0] = anchorRight->getEdge(1);
                vertices[8] = anchorRight->getEdge(1);
                return;
            }
            anchorRight->vertices[0] = val;
            anchorRight->vertices[8] = val;
            return;
        } else if (nextRight == 3) {
            if (anchorRight->fixedLeft == true) {
                vertices[0] = anchorRight->getEdge(3);
                vertices[8] = anchorRight->getEdge(3);
                return;
            }
            //setEdge(e, anchorRight->getEdge(3));
            anchorRight->vertices[16] = val;
            anchorRight->vertices[24] = val;
            return;
        }
    } else
    if (e == 2) {
        if (ancBottom != true) {
            return;
        }
        if ((anchorBottom->fixedSize == true) || (anchorBottom->fixedY == true)) {
            vertices[9] = anchorBottom->getEdge(nextBottom);
            vertices[17] = anchorBottom->getEdge(nextBottom);
            return;
        }
        if (nextBottom == 2) {
            if (anchorBottom->fixedBottom == true) {
                vertices[9] = anchorBottom->getEdge(2);
                vertices[17] = anchorBottom->getEdge(2);
                return;
            }
            anchorBottom->vertices[9] = val;
            anchorBottom->vertices[17] = val;
            return;
        } else if (nextTop == 0) {
            if (anchorBottom->fixedTop == true) {
                vertices[9] = anchorBottom->getEdge(0);
                vertices[17] = anchorBottom->getEdge(0);
                return;
            }
            anchorBottom->vertices[1] = val;
            anchorBottom->vertices[25] = val;
            return;
        }
    } else
    if (e == 3) {
        if (ancLeft != true) {
            return;
        }
        if ((anchorLeft->fixedSize == true) || (anchorLeft->fixedX == true)) {
            vertices[16] = anchorLeft->getEdge(nextLeft);
            vertices[24] = anchorLeft->getEdge(nextLeft);
            return;
        }
        if (nextLeft == 3) {
            if (anchorLeft->fixedLeft == true) {
                vertices[16] = anchorLeft->getEdge(3);
                vertices[24] = anchorLeft->getEdge(3);
                return;
            }
            //setEdge(e, anchorLeft->getEdge(3));
            anchorLeft->vertices[16] = val;
            anchorLeft->vertices[24] = val;
            return;
        } else if (nextLeft == 1) {
            if (anchorLeft->fixedRight == true) {
                vertices[16] = anchorLeft->getEdge(1);
                vertices[24] = anchorLeft->getEdge(1);
                return;
            }
            anchorLeft->vertices[0] = val;
            anchorLeft->vertices[8] = val;
            return;
        }
    }
}*/

void GUI_BOX::setFillColor (float r, float g, float b) {
    setVertexColor(0, r, g, b);
    setVertexColor(1, r, g, b);
    setVertexColor(2, r, g, b);
    setVertexColor(3, r, g, b);
}

float GUI_BOX::getEdge (int e) {
    //std::cout << "#" << e << std::endl;
    if (e == GUI_TOP) {
        //std::cout << " -- " << vertices[1] << std::endl;
        return vertices[1];
    } else if (e == GUI_RIGHT) {
        //std::cout << " -- " << vertices[0] << std::endl;
        return vertices[0];
    } else if (e == GUI_BOTTOM) {
        //std::cout << " -- " << vertices[9] << std::endl;
        return vertices[9];
    } else if (e == GUI_LEFT) {
        //std::cout << " -- " << vertices[16] << std::endl;
        return vertices[16];
    }
    //std::cout << "shouldn't be here" << std::endl;
    return 0.0f;
}

void GUI_BOX::anchorEdge (int e1, GUI_BOX *b, int e2) {
    anchors[e1]->pntrs.push_back(b);
    anchors[e1]->sides.push_back(e2);
    b->anchors[e2]->pntrs.push_back(this);
    b->anchors[e2]->sides.push_back(e1);

    if (e1 == GUI_TOP) {
        ancTop = true;
        anchorTop = b;
        nextTop = e2;
        if (e2 == GUI_TOP) {
            b->ancTop = true;
            b->anchorTop = this;
            b->nextTop = e1;
        } else
        if (e2 == GUI_BOTTOM) {
            b->ancBottom = true;
            b->anchorBottom = this;
            b->nextBottom = e1;
        }
    } else
    if (e1 == GUI_BOTTOM) {
        ancBottom = true;
        anchorBottom = b;
        nextBottom = e2;
        if (e2 == GUI_TOP) {
            b->ancTop = true;
            b->anchorTop = this;
            b->nextTop = e1;
        } else
        if (e2 == GUI_BOTTOM) {
            b->ancBottom = true;
            b->anchorBottom = this;
            b->nextBottom = e1;
        }
    } else
    if (e1 == GUI_RIGHT) {
        ancRight = true;
        anchorRight = b;
        nextRight = e2;
        if (e2 == GUI_RIGHT) {
            b->ancRight = true;
            b->anchorRight = this;
            b->nextRight = e1;
        } else
        if (e2 == GUI_LEFT) {
            b->ancLeft = true;
            b->anchorLeft = this;
            b->nextLeft = e1;
        }
    } else
    if (e1 == GUI_LEFT) {
        ancLeft = true;
        anchorLeft = b;
        nextLeft = e2;
        if (e2 == GUI_RIGHT) {
            b->ancRight = true;
            b->anchorRight = this;
            b->nextRight = e1;
        } else
        if (e2 == GUI_LEFT) {
            b->ancLeft = true;
            b->anchorLeft = this;
            b->nextLeft = e1;
        }
    }
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