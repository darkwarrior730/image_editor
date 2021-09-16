#include "gui_elements.hpp"
#include "image_handler.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <cstdint>
#include <queue>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ostream>
#include <string.h>

#include "shader_s.h"

int WIDTH = 640;
int HEIGHT = 480;

//bool lbutton_down = false;
//bool prevL = false;

bool dragging = false;

bool hidden = false;
//bool switchHide = false;
float preHide = 0.9f;

std::vector<GUI_BOX*> boxes;

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
            //switchHide = true;
            events.push(new event{"switch_hide_event", new bool(true)});
        }
    }
    //std::cout << mods << std::endl;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            //lbutton_down = true;
            //prevL = false;
            events.push(new event{"left_mouse_event", new bool(true)});
        } else if (action == GLFW_RELEASE) {
            //lbutton_down = false;
            //prevL = true;
            events.push(new event{"left_mouse_event", new bool(false)});
        }
        if (action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            ypos = HEIGHT - ypos;
            xpos = float(xpos/WIDTH)*2.0f - 1.0f;
            ypos = float(ypos/HEIGHT)*2.0f - 1.0f;
            //std::cout << "x:y -- " << xpos << ":" << ypos << std::endl;
            //std::cout << "in1" << std::endl;
            for (std::vector<GUI_BOX*>::reverse_iterator it = boxes.rbegin(); it != boxes.rend(); ++it) {
                //std::cout << "in2" << std::endl;
                //std::cout << (*it)->name << " : in" << std::endl;
                if ((*it)->checkCollide(xpos, ypos) == 1) {
                    //std::cout << "in3" << std::endl;
                    //clicked = *it;
                    events.push(new event{"button_click_event", (*it)});
                    break;
                }
            }
        }
    }
}

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
    glm::vec2 coord;
    float c_xfract, c_yfract;
    glm::vec2 c_coord;

    //GUI_BOX box1 = GUI_BOX();

    //box1.setShader("../src/vertex/test3.vs", "../src/fragment/test3.fs");

    GUI_TEXTURED_BOX box1 = GUI_TEXTURED_BOX("pencil3.jpg");//"bg.jpg");

    box1.box.setEdge(GUI_TOP, 0.9f);
    box1.box.setEdge(GUI_BOTTOM, -1.0f);
    box1.box.setEdge(GUI_RIGHT, 1.0f);
    box1.box.setEdge(GUI_LEFT, -0.8f);

    //box1.setFillColor(1.0f, 0.0f, 0.0f);

    box1.box.fixedRight = true;
    box1.box.fixedBottom = true;
    
    box1.box.updateVertexBuffer();

    GUI_BOX box2 = GUI_BOX();

    //box2.setShader("../src/vertex/test3.vs", "../src/fragment/test3.fs");

    box2.setEdge(GUI_TOP, 0.9f);
    box2.setEdge(GUI_BOTTOM, -1.0f);
    box2.setEdge(GUI_RIGHT, -0.8f);
    box2.setEdge(GUI_LEFT, -1.0f);
    
    box2.setFillColor(0.5f, 0.5f, 0.5f);

    box2.fixedBottom = true;
    box2.fixedLeft = true;

    box2.updateVertexBuffer();

    GUI_BOX box3 = GUI_BOX();

    //box3.setShader("../src/vertex/test3.vs", "../src/fragment/test3.fs");

    box3.setEdge(GUI_TOP, 1.0f);
    box3.setEdge(GUI_BOTTOM, 0.9f);
    box3.setEdge(GUI_RIGHT, 1.0f);
    box3.setEdge(GUI_LEFT, -1.0f);

    box3.setFillColor(0.0f, 0.0f, 0.0f);

    box3.fixedX = true;
    box3.fixedTop = true;
    
    box3.updateVertexBuffer();

    GUI_TEXTURED_BOX box4 = GUI_TEXTURED_BOX("pencil3.jpg");

    box4.box.setEdge(GUI_TOP, 0.05f);
    box4.box.setEdge(GUI_BOTTOM, -0.05f);
    box4.box.setEdge(GUI_RIGHT, 0.05f);
    box4.box.setEdge(GUI_LEFT, -0.05f);

    box4.box.updateVertexBuffer();

    GUI_TEXTURED_BOX box5 = GUI_TEXTURED_BOX("pencil3.jpg");

    box5.box.setEdge(GUI_TOP, 0.05f);
    box5.box.setEdge(GUI_BOTTOM, -0.05f);
    box5.box.setEdge(GUI_RIGHT, 0.05f);
    box5.box.setEdge(GUI_LEFT, -0.05f);
    
    box5.box.updateVertexBuffer();

    GUI_TEXTURED_BOX box6 = GUI_TEXTURED_BOX("pencil3.jpg");

    box6.box.setEdge(GUI_TOP, 0.05f);
    box6.box.setEdge(GUI_BOTTOM, -0.05f);
    box6.box.setEdge(GUI_RIGHT, 0.05f);
    box6.box.setEdge(GUI_LEFT, -0.05f);
    
    box6.box.updateVertexBuffer();

    box1.box.anchorEdge(GUI_TOP, &box3, GUI_BOTTOM);
    box2.anchorEdge(GUI_TOP, &box3, GUI_BOTTOM);
    box1.box.anchorEdge(GUI_LEFT, &box2, GUI_RIGHT);

    box4.box.addRel(&box2, 0.2f, 0.8f);
    box5.box.addRel(&box2, 0.7f, 0.8f);
    box6.box.addRel(&box2, 0.2f, 0.7f);

    box1.box.name = "box1";
    box2.name = "box2";
    box3.name = "box3";
    box4.box.name = "box4";
    box5.box.name = "box5";
    box6.box.name = "box6";

    boxes.push_back(&box1.box);
    boxes.push_back(&box2);
    boxes.push_back(&box3);
    boxes.push_back(&box4.box);
    boxes.push_back(&box5.box);
    boxes.push_back(&box6.box);

    GUI_BOX *bb;
    int be;

    int temp1, temp2;

    while (!glfwWindowShouldClose(window)) {

        //std::cout << "-----------------" << std::endl;

        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = HEIGHT - ypos;
        xfract = float(xpos/WIDTH);//*2.0f - 1.0f;
        yfract = float(ypos/HEIGHT);//*2.0f - 1.0f;
        coord = glm::vec2(xfract, yfract);
        c_xfract = xfract*2.0f - 1.0f;
        c_yfract = yfract*2.0f - 1.0f;
        c_coord = glm::vec2(c_xfract, c_yfract);

        if ((glfwGetWindowAttrib(window, GLFW_HOVERED) == false) && (dragging == true)) {
            //lbutton_down = false;
            //prevL = true;
            events.push(new event{"left_mouse_event", new bool(false)});
        }

        while (!events.empty()) {
            event *current_event = events.front();
            events.pop();
            std::cout << current_event->name << std::endl;
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
                std::cout << "button name : " << ((GUI_BOX*)current_event->data)->name << std::endl;
            } else if (strcmp(current_event->name, "left_mouse_event") == 0) {
                if (*(bool*)current_event->data == false) {
                    dragging = false;
                } else {
                    dragging = true;
                }
            }
        }

        /*if (clicked != nullptr) {
            std::cout << clicked->name << std::endl;
            clicked = nullptr;
        }*/

        /*if (switchHide == true) {
            switchHide = false;
            if (hidden == false) {
                preHide = box3.getEdge(GUI_BOTTOM);
                box3.setEdge(GUI_BOTTOM, 1.0f);
                hidden = true;
            } else {
                box3.setEdge(GUI_BOTTOM, preHide);
                hidden = false;
            }
        }*/

        if (dragging == true) {//lbutton_down == true) {
            //std::cout << "l down" << std::endl;

            be = -1;

            for (int i = 0; i < 4; i++) {
                temp1 = (i+1)%4;
                temp2 = (i-1)%4;
                if (temp2 == -1) {
                    temp2 = 3;
                }
                if (i%2 == 0) {
                    if (std::abs(box1.box.getEdge(i) - c_yfract) < 0.05f) {
                        //std::cout << "..." << (i+1)%4 << "==" << (i-1)%4 << std::endl;
                        if (fmin(box1.box.getEdge(temp1), box1.box.getEdge(temp2)) < c_xfract) {
                            if (fmax(box1.box.getEdge(temp1), box1.box.getEdge(temp2)) > c_xfract) {
                                bb = &box1.box;
                                be = i;
                                //std::cout << "box1 - " << i << std::endl; 
                                //std::cout << c_xfract << " - " << c_yfract << std::endl;
                                //printf("%f\n", fmin(box1.getEdge((i+1)%4), box1.getEdge((i-1)%4)));
                                //printf("%f\n", fmax(box1.getEdge((i+1)%4), box1.getEdge((i-1)%4)));
                                //std::cout << fmin(box1.getEdge((i+1)%4), box1.getEdge((i-1)%4)) << " - " << fmax(box1.getEdge((i+1)%4), box1.getEdge((i-1)%4)) << std::endl;
                                break;
                            }
                        }
                    }
                    if (std::abs(box2.getEdge(i) - c_yfract) < 0.05f) {
                        if (fmin(box2.getEdge(temp1), box2.getEdge(temp2)) < c_xfract) {
                            if (fmax(box2.getEdge(temp1), box2.getEdge(temp2)) > c_xfract) {
                                bb = &box2;
                                be = i;
                                //std::cout << "box2 - " << i << std::endl;
                                break;
                            }
                        }
                    }
                    if (std::abs(box3.getEdge(i) - c_yfract) < 0.05f) {
                        if (fmin(box3.getEdge(temp1), box3.getEdge(temp2)) < c_xfract) {
                            if (fmax(box3.getEdge(temp1), box3.getEdge(temp2)) > c_xfract) {
                                bb = &box3;
                                be = i;
                                //std::cout << "box3 - " << i << std::endl;
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
                                //std::cout << "box1 - " << i << std::endl;
                                break;
                            }
                        }
                    }
                    if (std::abs(box2.getEdge(i) - c_xfract) < 0.05f) {
                        if (fmin(box2.getEdge(temp1), box2.getEdge(temp2)) < c_yfract) {
                            if (fmax(box2.getEdge(temp1), box2.getEdge(temp2)) > c_yfract) {
                                bb = &box2;
                                be = i;
                                //std::cout << "box2 - " << i << std::endl;
                                break;
                            }
                        }
                    }
                    if (std::abs(box3.getEdge(i) - c_xfract) < 0.05f) {
                        if (fmin(box3.getEdge(temp1), box3.getEdge(temp2)) < c_yfract) {
                            if (fmax(box3.getEdge(temp1), box3.getEdge(temp2)) > c_yfract) {
                                bb = &box3;
                                be = i;
                                //std::cout << "box3 - " << i << std::endl;
                                break;
                            }
                        }
                    }
                }
            }

            if (be != -1) {
                //std::cout << "near edge" << std::endl;
                //std::cout << be << " - " << c_xfract << " - " << c_yfract << std::endl;
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

        box4.draw();
        box5.draw();
        box6.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
