#include "gui_elements.hpp"
#include "image_handler.hpp"
#include "drawing_space.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <cstdint>
#include <math.h>
#include <queue>
#include <ostream>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader_s.h"

int WIDTH = 640;
int HEIGHT = 480;

drawingSpace *ds;

bool dragging = false;

enum dm {MODE_DRAG, MODE_BRUSH, MODE_LINE} mode = MODE_DRAG; 

std::vector<GUI_BUTTON*> buttons;

GUI_BOX *clicked = nullptr;

typedef struct event{
    const char *name;
    void *data;
} event;

std::queue<event*> events;

int linestep = 0;
int line_x = 0;
int line_y = 0;

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

void plotLineLow(drawingSpace *ds, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y0;
    for (int x = x0; x < x1; x++) {
        ds->setPixel(x, y, r, g, b);
        if (D > 0) {
            y = y + yi;
            D = D + (2 * (dy - dx));
        } else {
            D = D + 2 * dy;
        }
    }
}

void plotLineHigh(drawingSpace *ds, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = x0;
    for (int y = y0; y < y1; y++) {
        ds->setPixel(x, y, r, g, b);
        if (D > 0) {
            x = x + xi;
            D = D + (2 * (dx - dy));
        } else {
            D = D + 2 * dx;
        }
    }
}

void plotLine(drawingSpace *ds, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            plotLineLow(ds, x1, y1, x0, y0, r, g, b);
        } else {
            plotLineLow(ds, x0, y0, x1, y1, r, g, b);
        }
    } else {
        if (y0 > y1) {
            plotLineHigh(ds, x1, y1, x0, y0, r, g, b);
        } else {
            plotLineHigh(ds, x0, y0, x1, y1, r, g, b);
        }
    }
}

void setModeDrag() {
    mode = MODE_DRAG;
}

void setModeBrush() {
    mode = MODE_BRUSH;
}

void setModeLine() {
    mode = MODE_LINE;
    linestep = 0;
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

    ds = new drawingSpace();
    ds->createBlankCanvas(128, 128);
    //ds->createImageCanvas("bg.jpg");

    ds->gui.box.setEdge(GUI_TOP, 0.9f);
    ds->gui.box.setEdge(GUI_LEFT, -0.8);

    ds->gui.name = (char*)"drawing";

    GUI_BOX box1 = GUI_BOX();
    box1.setEdge(GUI_BOTTOM, 0.9f);
    box1.setFillColor(1.0f, 0.0f, 0.0f);

    box1.anchorEdge(GUI_BOTTOM, &(ds->gui.box), GUI_TOP);

    box1.name = (char*)"menubar";

    GUI_BOX box2 = GUI_BOX();
    box2.setEdge(GUI_RIGHT, -0.8f);
    box2.setFillColor(0.5f, 0.5f, 0.5f);

    //box2.anchorEdge(GUI_RIGHT, &(ds->gui.box), GUI_LEFT);

    box2.name = (char*)"buttonsmenu";

    GUI_BUTTON brushbutton = GUI_BUTTON("pencil3.jpg", "pencil.jpg", setModeBrush, setModeDrag);

    brushbutton.box.box.setEdge(GUI_TOP, 0.05f);
    brushbutton.box.box.setEdge(GUI_BOTTOM, -0.05f);
    brushbutton.box.box.setEdge(GUI_RIGHT, 0.05f);
    brushbutton.box.box.setEdge(GUI_LEFT, -0.05f);

    brushbutton.box.box.updateVertexBuffer();

    brushbutton.box.box.addRel(&box2, 0.5f, 0.5f);

    brushbutton.name = (char*)"brush mode button";

    buttons.push_back(&brushbutton);

    GUI_BUTTON linebutton = GUI_BUTTON("pencil3.jpg", "pencil.jpg", setModeLine, setModeDrag);

    linebutton.box.box.setEdge(GUI_TOP, 0.05f);
    linebutton.box.box.setEdge(GUI_BOTTOM, -0.05f);
    linebutton.box.box.setEdge(GUI_RIGHT, 0.05f);
    linebutton.box.box.setEdge(GUI_LEFT, -0.05f);

    linebutton.box.box.updateVertexBuffer();

    linebutton.box.box.addRel(&box2, 0.5f, 0.25f);

    linebutton.name = (char*)"line mode button";

    buttons.push_back(&linebutton);

    //plotLine(ds, 0, 0, ds->canvas->width, ds->canvas->height, 0, 0, 0);

    double xpos, ypos;
    float xfract, yfract;
    float c_xfract, c_yfract;

    //int x = 1;

    while (!glfwWindowShouldClose(window)) {
        //std::cout << "x: " << x++ << std::endl;
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
            if (strcmp(current_event->name, "button_click_event") == 0) {
                GUI_BUTTON *temp = (GUI_BUTTON*)current_event->data;
                //std::cout << "button name : " << temp->name << std::endl;
                temp->click();
            } else if (strcmp(current_event->name, "left_mouse_event") == 0) {
                if (*(bool*)current_event->data == false) {
                    dragging = false;
                } else {
                    dragging = true;
                    if (mode == MODE_LINE) {
                        float tempy = (c_yfract - ds->gui.box.getEdge(GUI_BOTTOM))/(ds->gui.box.getEdge(GUI_TOP) - ds->gui.box.getEdge(GUI_BOTTOM));
                        float tempx = (c_xfract - ds->gui.box.getEdge(GUI_RIGHT))/(ds->gui.box.getEdge(GUI_LEFT) - ds->gui.box.getEdge(GUI_RIGHT));
                        //std::cout << tempx << ", " << tempy << std::endl;
                        if (linestep == 0) {
                            line_x = ds->canvas->width*(1.0f-tempx);
                            line_y = ds->canvas->height*(1.0f-tempy);
                            linestep = 1;
                        } else {
                            linestep = 0;
                            plotLine(ds, (int)line_x, (int)line_y, (int)ds->canvas->width*(1.0f-tempx), (int)ds->canvas->height*(1.0f-tempy), 0, 0, 0);
                        }
                    }
                }
            }
        }

        if (dragging == true) {
            if (mode == MODE_BRUSH) {
                if (ds->gui.checkCollide(c_xfract, c_yfract)) {
                    float tempy = (c_yfract - ds->gui.box.getEdge(GUI_BOTTOM))/(ds->gui.box.getEdge(GUI_TOP) - ds->gui.box.getEdge(GUI_BOTTOM));
                    float tempx = (c_xfract - ds->gui.box.getEdge(GUI_RIGHT))/(ds->gui.box.getEdge(GUI_LEFT) - ds->gui.box.getEdge(GUI_RIGHT));
                    for (int i = ds->canvas->width*(1.0f-tempx)-10; i < ds->canvas->width*(1.0f-tempx)+10; ++i) {
                        for (int j = ds->canvas->height*(1.0f-tempy)-10; j < ds->canvas->height*(1.0f-tempy)+10; ++j) {
                            if ((i >= 0 && i < ds->canvas->width) && (j >= 0 && j < ds->canvas->height)) {
                                ds->setPixel((int)i, (int)j, 0, 0, 0);
                            }
                        }
                    }
                }
            }
        }
        //std::cout << dragging << std::endl;

        glClearColor(1.0f, 1.0f, 1.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ds->draw();
        box1.draw();
        box2.draw();
        brushbutton.draw();
        linebutton.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
