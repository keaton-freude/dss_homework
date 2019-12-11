#include <iostream>
#include "httplib.h"
#include "json.hpp"
#include <GLFW/glfw3.h>
#include "gl/GL.h"
#include "glm/vec3.hpp"

using json = nlohmann::json;

int main() {
    GLFWwindow *window;

    glm::vec3 test(0.0f, 0.0f, 0.0f);

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
}