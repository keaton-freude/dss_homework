#include <iostream>
#include "httplib.h"
#include "json.hpp"
#include <GLFW/glfw3.h>
#include "gl/GL.h"
#include "glm/vec3.hpp"
#include "Window.h"

using json = nlohmann::json;
using namespace dss;

int main() {
    Window window(1280, 720, "DSS Homework");

    while (!glfwWindowShouldClose(window.GetWindow())) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window.GetWindow());

        glfwPollEvents();
    }

    glfwTerminate();
}