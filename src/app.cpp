#include "image_handler.hpp"
#include "drawing_space.hpp"

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

/*bool lbutton_down = false;
bool prevL = false;*/

/*bool press_plus = false;
bool press_minus = false;*/

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
        /*if (key == GLFW_KEY_EQUAL && mods == 1) {
            press_plus = true;
        }
        if (key == GLFW_KEY_MINUS && mods == 0) {
            press_minus = true;
        }*/
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    /*if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            lbutton_down = true;
            prevL = false;
        } else if (action == GLFW_RELEASE) {
            lbutton_down = false;
            prevL = true;
        }
    }*/
}

/*void handle_zoom (GLFWwindow *window, unsigned int VAO, unsigned int VBO, float vertices[], size_t vs, float& scale)
{
    float temp;
    if (press_plus == true) {
        press_plus = false;
        scale *= 2.0f;
        temp = (1.0f - (1.0f / scale))/2;

        //std::cout << scale << " - " << temp << std::endl;

        vertices[6] = 1.0f-temp;
        vertices[7] = 1.0f-temp;
        
        vertices[8+6] = 1.0f-temp;
        vertices[8+7] = temp;
        
        vertices[16+6] = temp;
        vertices[16+7] = temp;

        vertices[24+6] = temp;
        vertices[24+7] = 1.0f-temp;

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vs, vertices, GL_DYNAMIC_DRAW);
    }
    if (press_minus == true) {
        press_minus = false;
        
        //std::cout << scale << " - " << temp << std::endl;

        if (scale == 1.0f) {
            return;
        }

        scale /= 2.0f;
        temp = (1.0f - (1.0f / scale))/2;

        std::cout << scale << " - " << temp << std::endl;

        vertices[6] = 1.0f-temp;
        vertices[7] = 1.0f-temp;
        
        vertices[8+6] = 1.0f-temp;
        vertices[8+7] = temp;
        
        vertices[16+6] = temp;
        vertices[16+7] = temp;

        vertices[24+6] = temp;
        vertices[24+7] = 1.0f-temp;

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vs, vertices, GL_DYNAMIC_DRAW);
    }
    return;
}*/

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

    drawingSpace ds = drawingSpace();

    while (!glfwWindowShouldClose(window)) {

        glClearColor(1.0f, 1.0f, 1.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ds.gui.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ds.gui.img->writePixels();

    ds.gui.img->freeImage();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
