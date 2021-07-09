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

bool lbutton_down = false;
bool prevL = false;

bool press_plus = false;
bool press_minus = false;

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
        if (key == GLFW_KEY_EQUAL && mods == 1) {
            press_plus = true;
        }
        if (key == GLFW_KEY_MINUS && mods == 0) {
            press_minus = true;
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

void handle_zoom (GLFWwindow* window, unsigned int VAO, unsigned int VBO, float vertices[], size_t vs, float& scale)
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

    /*Shader testShader(
        "../src/vertex/test2.vs",
        "../src/fragment/test2.fs");

    float scale = 1.0f;

    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
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
    glEnableVertexAttribArray(2);*/

    drawingSpace ds = drawingSpace();

    /*int width, height, bpp;

    uint8_t* pixels = stbi_load("bg.jpg", &width, &height, &bpp, 3);

    //std::cout << width << std::endl;
    //std::cout << height << std::endl;
    //std::cout << bpp << std::endl;

    //WIDTH = width;
    //HEIGHT = height;
    //glViewport(0, 0, width, height);

    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);*/

    //int i = 0;

    double xpos, ypos;
    float xfract, yfract;
    glm::vec2 coord;
    float c_xfract, c_yfract;
    glm::vec2 c_coord;

    int temp;

    float prevtime = glfwGetTime();
    float time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();
        //std::cout << 1.0f/(time-prevtime) << std::endl;
        prevtime = time;

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

        handle_zoom(window, ds.gui.box.VAO, ds.gui.box.VBO, ds.gui.box.vertices, sizeof(ds.gui.box.vertices), ds.zoom_scale);

        if (lbutton_down != prevL) {
            if (lbutton_down == true) {
                for (int a = ds.gui.img.width*(xfract)-5; a < ds.gui.img.width*(xfract)+5; a++) {
                    for (int b = ds.gui.img.height*(1.0f-yfract)-5; b < ds.gui.img.height*(1.0f-yfract)+5; b++) {
                        temp = b*ds.gui.img.width + a;
                        temp *= ds.gui.img.bpp;
                        *(ds.gui.img.pixels + temp) = 0;
                        *(ds.gui.img.pixels + temp + 1) = 0;
                        *(ds.gui.img.pixels + temp + 2) = 0;
                    }
                }
            }
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        /*glBindTexture(GL_TEXTURE_2D, ds.texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ds.img.width, ds.img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ds.img.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);*/

        // render container
        /*ds.testShader.use();
        glBindVertexArray(ds.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

        ds.gui.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ds.gui.img.writePixels();

    ds.gui.img.freeImage();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
