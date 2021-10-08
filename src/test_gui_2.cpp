#include "new_gui_elements.hpp"
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

int do_a_thing = 0;

BUTTON *b;

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
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            ypos = HEIGHT - ypos;
            xpos = float(xpos/WIDTH)*2.0f - 1.0f;
            ypos = float(ypos/HEIGHT)*2.0f - 1.0f;
            if (b->checkCollide(xpos, ypos) == true) {
                b->changeState();
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

    BASE_BOX *box = new COLORED_BOX(-1.0f, 1.0f, 2.0f, 2.0f, color{0.5f, 0.2f, 1.0f, 1.0f});
    BASE_BOX *box2 = new TEXTURED_BOX(-0.5f, 0.5f, 1.0f, 1.0f, "pencil.jpg", 0.0f, 1.0f, 1.0f, 1.0f);
    BASE_BOX *box3 = new TEXTURED_BOX(-0.5f, 0.5f, 1.0f, 1.0f, "pencil3.jpg", 0.0f, 1.0f, 1.0f, 1.0f);

    BUTTON *but = new BUTTON(box2, box3);
    b = but;

    while (!glfwWindowShouldClose(window)) {
        box->draw();
        but->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
