#include "Window.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace dss;

Window::Window(uint16_t width, uint16_t height, const std::string &name)
    : _width(width), _height(height), _name(name)  {

    if (!glfwInit()) {
        // TODO: Same as below.. makes sense for this to be catastrophic?
        std::cerr << "Failed to initialize GLFW! Giving up..." << std::endl;
        std::abort();
    }

    _window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    if (!_window) {
        // TODO: Likely catastrophic and probably a matter of taste, so consider not exiting
        std::cerr << "Failed to create GLFW Window! Giving up..." << std::endl;
        // TODO: Is it okay to call Terminate multiple times? In case someone else calls it while handling
        // maybe a SIG ABORT?
        glfwTerminate();
        std::abort();
    }

    glfwMakeContextCurrent(_window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW! Giving up..." << std::endl;
        std::abort();
    }
}

Window::~Window() {
    glfwTerminate();
}

GLFWwindow *Window::GetWindow() {
    return _window;
}

bool Window::Running() const {
    // Below will return true after someone has told _us_ to close the window
    // or when the user presses something like the 'X' exit button in their windowing
    // system
    return !glfwWindowShouldClose(_window);
}

void Window::SwapBuffers() const {
    glfwSwapBuffers(_window);
}

void Window::Close() const {
    glfwSetWindowShouldClose(_window, 1);
}

uint32_t Window::Width() const {
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return static_cast<uint32_t>(width);
}

uint32_t Window::Height() const {
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return static_cast<uint32_t>(height);
}