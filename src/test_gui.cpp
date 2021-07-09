#include "gui_elements.hpp"
#include "image_handler.hpp"

//#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <cstdint>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ostream>

#include "shader_s.h"

int WIDTH = 640;
int HEIGHT = 480;

bool lbutton_down = false;
bool prevL = false;

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            lbutton_down = true;
            prevL = false;
        } else if (action == GLFW_RELEASE) {
            lbutton_down = false;
            prevL = true;
        }
    }
}

int main(int argc, char *argv[])
{
    if (!glfwInit()) {
        return 1;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "My Title", NULL, NULL);
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

    GUI_BOX box1 = GUI_BOX();

    box1.setShader("../src/vertex/test3.vs", "../src/fragment/test3.fs");

    box1.setEdge(GUI_TOP, 0.9f);
    box1.setEdge(GUI_BOTTOM, -1.0f);
    box1.setEdge(GUI_RIGHT, 1.0f);
    box1.setEdge(GUI_LEFT, -0.8f);

    box1.setFillColor(1.0f, 0.0f, 0.0f);

    box1.fixedRight = true;
    box1.fixedBottom = true;
    
    box1.updateVertexBuffer();

    GUI_BOX box2 = GUI_BOX();

    box2.setShader("../src/vertex/test3.vs", "../src/fragment/test3.fs");

    box2.setEdge(GUI_TOP, 0.9f);
    box2.setEdge(GUI_BOTTOM, -1.0f);
    box2.setEdge(GUI_RIGHT, -0.8f);
    box2.setEdge(GUI_LEFT, -1.0f);
    
    box2.setFillColor(0.5f, 0.5f, 0.5f);

    box2.fixedBottom = true;
    box2.fixedLeft = true;

    box2.updateVertexBuffer();

    GUI_BOX box3 = GUI_BOX();

    box3.setShader("../src/vertex/test3.vs", "../src/fragment/test3.fs");

    box3.setEdge(GUI_TOP, 1.0f);
    box3.setEdge(GUI_BOTTOM, 0.9f);
    box3.setEdge(GUI_RIGHT, 1.0f);
    box3.setEdge(GUI_LEFT, -1.0f);

    box3.setFillColor(0.0f, 0.0f, 0.0f);

    box3.fixedX = true;
    box3.fixedTop = true;
    
    box3.updateVertexBuffer();

    GUI_BOX box4 = GUI_BOX();

    box4.setShader("../src/vertex/test3.vs", "../src/fragment/test3.fs");

    box4.setEdge(GUI_TOP, 0.9f);
    box4.setEdge(GUI_BOTTOM, -1.0f);
    box4.setEdge(GUI_RIGHT, 1.0f);
    box4.setEdge(GUI_LEFT, -1.0f);

    box4.setFillColor(1.0f, 1.0f, 1.0f);

    box4.fixedX = true;
    box4.fixedBottom = true;
    
    box4.updateVertexBuffer();

    //box1.anchorEdge(GUI_TOP, &box3, GUI_BOTTOM);
    //box2.anchorEdge(GUI_TOP, &box3, GUI_BOTTOM);
    //box1.anchorEdge(GUI_LEFT, &box2, GUI_RIGHT);

    box4.anchorEdge(GUI_TOP, &box3, GUI_BOTTOM);
    box1.anchorEdge(GUI_RIGHT, &box4, GUI_RIGHT);
    box2.anchorEdge(GUI_LEFT, &box4, GUI_LEFT);
    box1.anchorEdge(GUI_LEFT, &box2, GUI_RIGHT);

    GUI_BOX* bb;
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

        if (glfwGetWindowAttrib(window, GLFW_HOVERED) == false) {
            lbutton_down = false;
            prevL = true;
        }

        if (lbutton_down == true) {
            //std::cout << "l down" << std::endl;

            be = -1;

            for (int i = 0; i < 4; i++) {
                temp1 = (i+1)%4;
                temp2 = (i-1)%4;
                if (temp2 == -1) {
                    temp2 = 3;
                }
                if (i%2 == 0) {
                    if (std::abs(box1.getEdge(i) - c_yfract) < 0.05f) {
                        //std::cout << "..." << (i+1)%4 << "==" << (i-1)%4 << std::endl;
                        if (fmin(box1.getEdge(temp1), box1.getEdge(temp2)) < c_xfract) {
                            if (fmax(box1.getEdge(temp1), box1.getEdge(temp2)) > c_xfract) {
                                bb = &box1;
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
                    if (std::abs(box1.getEdge(i) - c_xfract) < 0.05f) {
                        if (fmin(box1.getEdge(temp1), box1.getEdge(temp2)) < c_yfract) {
                            if (fmax(box1.getEdge(temp1), box1.getEdge(temp2)) > c_yfract) {
                                bb = &box1;
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

        //box1.draw();
        //box2.draw();
        //box3.draw();

        box4.draw();
        box3.draw();
        box2.draw();
        box1.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
