#include "gui_elements.hpp"
#include "image_handler.hpp"
#include "drawing_space.hpp"

#include <algorithm>
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

enum dm {MODE_DRAG, MODE_BRUSH, MODE_LINE, MODE_FILL} mode = MODE_DRAG; 

std::vector<GUI_BUTTON*> buttons;

std::vector<GUI_INPUT_TEXT*> input_text;

GUI_INPUT_TEXT *cur_active_text_input = nullptr;

GUI_BOX *clicked = nullptr;

// CHANGE EVENTS NAME FROM STRING TO ENUM

typedef struct event{
    const char *name;
    void *data;
} event;

std::queue<event*> events;

int linestep = 0;
int line_x = 0;
int line_y = 0;

color current_color = {0, 0, 0};

GUI_BUTTON *currently_down = nullptr;

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
        } else {
            events.push(new event{"key_event", new int(key)});
        }
        if (cur_active_text_input != nullptr) {
            cur_active_text_input->add_character(key);
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
            for (std::vector<GUI_INPUT_TEXT*>::reverse_iterator it = input_text.rbegin(); it != input_text.rend(); ++it) {
                if ((*it)->checkCollide(xpos, ypos) == 1) {
                    events.push(new event{"input_text_click_event", (*it)});
                    // break;
                    return;
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

typedef struct point {int x; int y;} point;

void floodFill(drawingSpace *ds, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    std::queue<point*> toVisit;
    toVisit.push(new point{x, y});
    point *temp;
    color t;
    color c = ds->getPixel(x, y);
    while (!toVisit.empty()) {
        temp = toVisit.front();
        t = ds->getPixel(temp->x, temp->y);
        if (t.r == c.r && t.g == c.g && t.b == c.b) {
            ds->setPixel(temp->x, temp->y, r, g, b);
            // need to perform bounds checking
            if (x+1 < ds->canvas->width) toVisit.push(new point{temp->x+1, temp->y});
            if (x-1 >= 0) toVisit.push(new point{temp->x-1, temp->y});
            if (y+1 < ds->canvas->height) toVisit.push(new point{temp->x, temp->y+1});
            if (y-1 >= 0) toVisit.push(new point{temp->x, temp->y-1});
        }
        toVisit.pop();
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

void setModeFill() {
    mode = MODE_FILL;
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

    GUI_INPUT_TEXT text_box = GUI_INPUT_TEXT(-0.8f, 0.9f, 1.8f, 1.9f, 80, 50);
    input_text.push_back(&text_box);

    GUI_BOX box1 = GUI_BOX();
    box1.setEdge(GUI_BOTTOM, 0.9f);
    box1.setFillColor(1.0f, 0.0f, 0.0f);

    box1.anchorEdge(GUI_BOTTOM, &(ds->gui.box), GUI_TOP);

    box1.name = (char*)"menubar";

    GUI_BOX box2 = GUI_BOX();
    box2.setEdge(GUI_RIGHT, -0.8f);
    box2.setEdge(GUI_TOP, 0.9f);
    box2.setFillColor(0.5f, 0.5f, 0.5f);

    box2.anchorEdge(GUI_RIGHT, &(ds->gui.box), GUI_LEFT);
    box2.anchorEdge(GUI_TOP, &box1, GUI_BOTTOM);

    box2.name = (char*)"buttonsmenu";

    GUI_BUTTON brushbutton = GUI_BUTTON("pencil3.jpg", "pencil.jpg", setModeBrush, setModeDrag);

    brushbutton.box.box.setEdge(GUI_TOP, 0.05f);
    brushbutton.box.box.setEdge(GUI_BOTTOM, -0.05f);
    brushbutton.box.box.setEdge(GUI_RIGHT, 0.05f);
    brushbutton.box.box.setEdge(GUI_LEFT, -0.05f);

    brushbutton.box.box.updateVertexBuffer();

    brushbutton.box.box.addRel(&box2, 0.5f, 0.75f);

    brushbutton.name = (char*)"brush mode button";

    buttons.push_back(&brushbutton);

    GUI_BUTTON linebutton = GUI_BUTTON("pencil3.jpg", "pencil.jpg", setModeLine, setModeDrag);

    linebutton.box.box.setEdge(GUI_TOP, 0.05f);
    linebutton.box.box.setEdge(GUI_BOTTOM, -0.05f);
    linebutton.box.box.setEdge(GUI_RIGHT, 0.05f);
    linebutton.box.box.setEdge(GUI_LEFT, -0.05f);

    linebutton.box.box.updateVertexBuffer();

    linebutton.box.box.addRel(&box2, 0.5f, 0.5f);

    linebutton.name = (char*)"line mode button";

    buttons.push_back(&linebutton);

    GUI_BUTTON fillbutton = GUI_BUTTON("pencil3.jpg", "pencil.jpg", setModeFill, setModeDrag);

    fillbutton.box.box.setEdge(GUI_TOP, 0.05f);
    fillbutton.box.box.setEdge(GUI_BOTTOM, -0.05f);
    fillbutton.box.box.setEdge(GUI_RIGHT, 0.05f);
    fillbutton.box.box.setEdge(GUI_LEFT, -0.05f);

    fillbutton.box.box.updateVertexBuffer();

    fillbutton.box.box.addRel(&box2, 0.5f, 0.25f);

    fillbutton.name = (char*)"fill mode button";

    buttons.push_back(&fillbutton);

    // GUI_TEXT hello = GUI_TEXT("the quick brown fox", -0.5f, 0.0f, 0.05f, 0.05f);
    // GUI_TEXT hello2 = GUI_TEXT("jumped over the lazy dog", -0.5f, -0.05f, 0.05f, 0.05f);

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
                if (currently_down != nullptr) currently_down->click();
                if (currently_down != temp) {
                    temp->click();
                    currently_down = temp;
                } else {
                    currently_down = nullptr;
                }
            } else if (strcmp(current_event->name, "key_event") == 0){
                if (*(int*)(current_event->data) == GLFW_KEY_W) {
                    current_color = {255, 255, 255};
                } else
                if (*(int*)(current_event->data) == GLFW_KEY_D) {
                    current_color = {0, 0, 0};
                } else
                if (*(int*)(current_event->data) == GLFW_KEY_R) {
                    current_color = {255, 0, 0};
                } else
                if (*(int*)(current_event->data) == GLFW_KEY_G) {
                    current_color = {0, 255, 0};
                } else
                if (*(int*)(current_event->data) == GLFW_KEY_B) {
                    current_color = {0, 0, 255};
                }
            } else if (strcmp(current_event->name, "left_mouse_event") == 0) {
                if (*(bool*)current_event->data == false) {
                    dragging = false;
                } else {
                    dragging = true;
                    float tempy = (c_yfract - ds->gui.box.getEdge(GUI_BOTTOM))/(ds->gui.box.getEdge(GUI_TOP) - ds->gui.box.getEdge(GUI_BOTTOM));
                    float tempx = (c_xfract - ds->gui.box.getEdge(GUI_RIGHT))/(ds->gui.box.getEdge(GUI_LEFT) - ds->gui.box.getEdge(GUI_RIGHT));
                    tempx = ds->canvas->width*(1.0f-tempx);
                    tempy = ds->canvas->height*(1.0f-tempy);
                    if (mode == MODE_LINE) {
                        if (ds->gui.checkCollide(c_xfract, c_yfract)) {
                            //std::cout << tempx << ", " << tempy << std::endl;
                            if (linestep == 0) {
                                line_x = tempx;
                                line_y = tempy;
                                linestep = 1;
                            } else {
                                linestep = 0;
                                plotLine(ds, (int)line_x, (int)line_y, (int)tempx, (int)tempy, current_color.r, current_color.g, current_color.b);
                            }
                        }
                    } else
                    if (mode == MODE_FILL) {
                        if (ds->gui.checkCollide(c_xfract, c_yfract)) {
                            floodFill(ds, tempx, tempy, current_color.r, current_color.g, current_color.b);
                        }
                    }
                }
            } else if (strcmp(current_event->name, "input_text_click_event") == 0) {
                if (cur_active_text_input == (GUI_INPUT_TEXT*)current_event->data) {
                    cur_active_text_input = nullptr;
                } else {
                    cur_active_text_input = (GUI_INPUT_TEXT*)current_event->data;
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
                                ds->setPixel((int)i, (int)j, current_color.r, current_color.g, current_color.b);
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
        fillbutton.draw();
        // hello.draw();
        // hello2.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
