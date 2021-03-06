#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "GlfwUserPointer.h"

using namespace dss;

Window::Window(uint16_t width, uint16_t height, const std::string &name)
    :   _userPointer(std::make_unique<GlfwUserPointer>()), 
        _width(width),
        _height(height)
{

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW! Giving up..." << std::endl;
        std::abort();
    }

    _window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    if (!_window) {
        std::cerr << "Failed to create GLFW Window! Giving up..." << std::endl;
        glfwTerminate();
        std::abort();
    }

    // GLFW lets us store a pointer to any object, 'GlfwUserPointer' will wrap up
    // the required pointers
    _userPointer->window = this;
    glfwSetWindowUserPointer(_window, _userPointer.get());

    glfwSetWindowSizeCallback(_window, [](GLFWwindow *window, int width, int height){
        void *userPointer = glfwGetWindowUserPointer(window);

        // Hmm, dynamic_cast wont work on a void*.. any nice way to ensure
        // below works..?
        GlfwUserPointer *glfwUserPointer = static_cast<GlfwUserPointer *>(userPointer);
        Window *ourWindow = glfwUserPointer->window;

        glViewport(0, 0, width, height);
        ourWindow->_width = width;
        ourWindow->_height = height;
        for(const auto& callback : ourWindow->_resizeCallbacks) {
            callback(WindowResizeEvent((int)width, (int)height));
        }
    });

    glfwMakeContextCurrent(_window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW! Giving up..." << std::endl;
        std::abort();
    }
}

Window::~Window() {
    glfwTerminate();
}

// Alert all observers of the new size so they can adjust themselves
void Window::WindowSizeCallback(GLFWwindow* window, int width, int height) {
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

void Window::RegisterResizeEventCallback(std::function<void(WindowResizeEvent)> callback) {
    _resizeCallbacks.push_back(callback);
}