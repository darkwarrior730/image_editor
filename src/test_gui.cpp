#include "gui_elements.hpp"
#include "image_handler.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <cstdint>
#include <math.h>
#include <queue>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ostream>
#include <string.h>

#include "shader_s.h"

int WIDTH = 640;
int HEIGHT = 480;

bool dragging = false;

bool hidden = false;
float preHide = 0.9f;

std::vector<GUI_BUTTON*> buttons;

GUI_BOX *clicked = nullptr;

typedef struct event{
    const char *name;
    void *data;
} event;

std::queue<event*> events;

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (mods == 2 && key == GLFW_KEY_H) {
            events.push(new event{"switch_hide_event", new bool(true)});
        }
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            events.push(new event{"left_mouse_event", new bool(true)});
        } else if (action == GLFW_RELEASE) {
            events.push(new event{"left_mouse_event", new bool(false)});
        }
        if (action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            ypos = HEIGHT - ypos;
            xpos = float(xpos/WIDTH)*2.0f - 1.0f;
            ypos = float(ypos/HEIGHT)*2.0f - 1.0f;
            for (std::vector<GUI_BUTTON*>::reverse_iterator it = buttons.rbegin(); it != buttons.rend(); ++it) {
                if ((*it)->checkCollide(xpos, ypos) == 1) {
                    events.push(new event{"button_click_event", (*it)});
                    break;
                }
            }
        }
    }
}

void doup() { std::cout << "i've been unpressed" << std::endl;}
void dodown() { std::cout << "i've been pressed down" << std::endl;}

int main(int argc, char *argv[])
{
    if (!glfwInit()) {
        return 1;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "My Title", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return 1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, key_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double xpos, ypos;
    float xfract, yfract;
    float c_xfract, c_yfract;

    GUI_TEXTURED_BOX box1 = GUI_TEXTURED_BOX("pencil3.jpg");

    box1.box.setEdge(GUI_TOP, 0.9f);
    box1.box.setEdge(GUI_BOTTOM, -1.0f);
    box1.box.setEdge(GUI_RIGHT, 1.0f);
    box1.box.setEdge(GUI_LEFT, -0.8f);

    box1.box.fixedRight = true;
    box1.box.fixedBottom = true;
    
    box1.box.updateVertexBuffer();

    GUI_BOX box2 = GUI_BOX();

    box2.setEdge(GUI_TOP, 0.9f);
    box2.setEdge(GUI_BOTTOM, -1.0f);
    box2.setEdge(GUI_RIGHT, -0.8f);
    box2.setEdge(GUI_LEFT, -1.0f);
    
    box2.setFillColor(0.5f, 0.5f, 0.5f);

    box2.fixedBottom = true;
    box2.fixedLeft = true;

    box2.updateVertexBuffer();

    GUI_BOX box3 = GUI_BOX();

    box3.setEdge(GUI_TOP, 1.0f);
    box3.setEdge(GUI_BOTTOM, 0.9f);
    box3.setEdge(GUI_RIGHT, 1.0f);
    box3.setEdge(GUI_LEFT, -1.0f);

    box3.setFillColor(0.0f, 0.0f, 0.0f);

    box3.fixedX = true;
    box3.fixedTop = true;
    
    box3.updateVertexBuffer();

    GUI_BUTTON button1 = GUI_BUTTON("pencil3.jpg", "pencil.jpg", dodown, doup);

    button1.box.box.setEdge(GUI_TOP, 0.05f);
    button1.box.box.setEdge(GUI_BOTTOM, -0.05f);
    button1.box.box.setEdge(GUI_RIGHT, 0.05f);
    button1.box.box.setEdge(GUI_LEFT, -0.05f);

    button1.box.box.updateVertexBuffer();

    GUI_BUTTON button2 = GUI_BUTTON("pencil3.jpg", "pencil.jpg", dodown, doup);

    button2.box.box.setEdge(GUI_TOP, 0.05f);
    button2.box.box.setEdge(GUI_BOTTOM, -0.05f);
    button2.box.box.setEdge(GUI_RIGHT, 0.05f);
    button2.box.box.setEdge(GUI_LEFT, -0.05f);

    button2.box.box.updateVertexBuffer();

    GUI_BUTTON button3 = GUI_BUTTON("pencil3.jpg", "pencil.jpg", dodown, doup);

    button3.box.box.setEdge(GUI_TOP, 0.05f);
    button3.box.box.setEdge(GUI_BOTTOM, -0.05f);
    button3.box.box.setEdge(GUI_RIGHT, 0.05f);
    button3.box.box.setEdge(GUI_LEFT, -0.05f);

    button3.box.box.updateVertexBuffer();

    box1.box.anchorEdge(GUI_TOP, &box3, GUI_BOTTOM);
    box2.anchorEdge(GUI_TOP, &box3, GUI_BOTTOM);
    box1.box.anchorEdge(GUI_LEFT, &box2, GUI_RIGHT);

    button1.box.box.addRel(&box2, 0.2f, 0.8f);
    button2.box.box.addRel(&box2, 0.7f, 0.8f);
    button3.box.box.addRel(&box2, 0.2f, 0.7f);

    box1.name = (char*)"box1";
    box2.name = (char*)"box2";
    box3.name = (char*)"box3";
    button1.name = (char*)"button1";
    button2.name = (char*)"button2";
    button3.name = (char*)"button3";

    buttons.push_back(&button1);
    buttons.push_back(&button2);
    buttons.push_back(&button3);

    GUI_BOX *bb;
    int be;

    int temp1, temp2;

    while (!glfwWindowShouldClose(window)) {
        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = HEIGHT - ypos;
        xfract = float(xpos/WIDTH);
        yfract = float(ypos/HEIGHT);
        c_xfract = xfract*2.0f - 1.0f;
        c_yfract = yfract*2.0f - 1.0f;

        if ((glfwGetWindowAttrib(window, GLFW_HOVERED) == false) && (dragging == true)) {
            events.push(new event{"left_mouse_event", new bool(false)});
        }

        while (!events.empty()) {
            event *current_event = events.front();
            events.pop();
            //std::cout << current_event->name << std::endl;
            if (strcmp(current_event->name, "switch_hide_event") == 0) {
                if (hidden == false) {
                    preHide = box3.getEdge(GUI_BOTTOM);
                    box3.setEdge(GUI_BOTTOM, 1.0f);
                    hidden = true;
                } else {
                    box3.setEdge(GUI_BOTTOM, preHide);
                    hidden = false;
                }
            } else if (strcmp(current_event->name, "button_click_event") == 0) {
                GUI_BUTTON *temp = (GUI_BUTTON*)current_event->data;
                //std::cout << "button name : " << temp->name << std::endl;
                temp->click();
            } else if (strcmp(current_event->name, "left_mouse_event") == 0) {
                if (*(bool*)current_event->data == false) {
                    dragging = false;
                } else {
                    dragging = true;
                }
            }
        }

        if (dragging == true) {

            be = -1;

            for (int i = 0; i < 4; i++) {
                temp1 = (i+1)%4;
                temp2 = (i-1)%4;
                if (temp2 == -1) {
                    temp2 = 3;
                }
                if (i%2 == 0) {
                    if (std::abs(box1.box.getEdge(i) - c_yfract) < 0.05f) {
                        if (fmin(box1.box.getEdge(temp1), box1.box.getEdge(temp2)) < c_xfract) {
                            if (fmax(box1.box.getEdge(temp1), box1.box.getEdge(temp2)) > c_xfract) {
                                bb = &box1.box;
                                be = i;
                                break;
                            }
                        }
                    }
                    if (std::abs(box2.getEdge(i) - c_yfract) < 0.05f) {
                        if (fmin(box2.getEdge(temp1), box2.getEdge(temp2)) < c_xfract) {
                            if (fmax(box2.getEdge(temp1), box2.getEdge(temp2)) > c_xfract) {
                                bb = &box2;
                                be = i;
                                break;
                            }
                        }
                    }
                    if (std::abs(box3.getEdge(i) - c_yfract) < 0.05f) {
                        if (fmin(box3.getEdge(temp1), box3.getEdge(temp2)) < c_xfract) {
                            if (fmax(box3.getEdge(temp1), box3.getEdge(temp2)) > c_xfract) {
                                bb = &box3;
                                be = i;
                                break;
                            }
                        }
                    }
                } else {
                    if (std::abs(box1.box.getEdge(i) - c_xfract) < 0.05f) {
                        if (fmin(box1.box.getEdge(temp1), box1.box.getEdge(temp2)) < c_yfract) {
                            if (fmax(box1.box.getEdge(temp1), box1.box.getEdge(temp2)) > c_yfract) {
                                bb = &box1.box;
                                be = i;
                                break;
                            }
                        }
                    }
                    if (std::abs(box2.getEdge(i) - c_xfract) < 0.05f) {
                        if (fmin(box2.getEdge(temp1), box2.getEdge(temp2)) < c_yfract) {
                            if (fmax(box2.getEdge(temp1), box2.getEdge(temp2)) > c_yfract) {
                                bb = &box2;
                                be = i;
                                break;
                            }
                        }
                    }
                    if (std::abs(box3.getEdge(i) - c_xfract) < 0.05f) {
                        if (fmin(box3.getEdge(temp1), box3.getEdge(temp2)) < c_yfract) {
                            if (fmax(box3.getEdge(temp1), box3.getEdge(temp2)) > c_yfract) {
                                bb = &box3;
                                be = i;
                                break;
                            }
                        }
                    }
                }
            }

            if (be != -1) {
                if (be%2 == 0) {
                    bb->setEdge(be, c_yfract);
                } else {
                    bb->setEdge(be, c_xfract);
                }
                bb->updateVertexBuffer();
            }

        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        box3.draw();
        box2.draw();
        box1.draw();

        button1.draw();
        button2.draw();
        button3.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    box1.img->freeImage();
    button1.up_image->freeImage();
    button1.down_image->freeImage();
    button2.up_image->freeImage();
    button2.down_image->freeImage();
    button3.up_image->freeImage();
    button3.down_image->freeImage();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
