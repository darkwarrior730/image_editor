#include "gui_elements.hpp"
#include "image_handler.hpp"

#include <cstddef>
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

    while (!glfwWindowShouldClose(window)) {
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

        glClearColor(1.0f, 1.0f, 1.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        box1.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
