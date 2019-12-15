#include "Input.h"
#include "GlfwUserPointer.h"

#include <GLFW/glfw3.h>

using namespace dss;

/**
 *  Specifically initializing this hash table in the .cpp, to avoid GLFW in the header
 */
const std::unordered_map<Input::Keys, int> Input::_KEY_SCAN_CODE_MAPPING = {
    {Input::Keys::Escape, GLFW_KEY_ESCAPE},
    {Input::Keys::L, GLFW_KEY_L},
    {Input::Keys::H, GLFW_KEY_H},
    {Input::Keys::Left, GLFW_KEY_LEFT},
    {Input::Keys::Right, GLFW_KEY_RIGHT},
    {Input::Keys::A, GLFW_KEY_A},
    {Input::Keys::D, GLFW_KEY_D}
};

Input::Input(std::shared_ptr<Window> window) : _window(window) {
    // Register ourself within the GLFW user pointer
    void *userPointer = glfwGetWindowUserPointer(_window->GetWindow());

    // Any safe way to cast this?
    GlfwUserPointer *glfwUserPointer = static_cast<GlfwUserPointer *>(userPointer);
    glfwUserPointer->input = this;
    glfwSetKeyCallback(_window->GetWindow(), Input::KeyCallback);
}

void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    void *userPointer = glfwGetWindowUserPointer(window);

    // Any safe way to cast this?
    GlfwUserPointer *glfwUserPointer = static_cast<GlfwUserPointer *>(userPointer);
    glfwUserPointer->input->HandleKeyCallback(key, scancode, action, mods);
}

void Input::HandleKeyCallback(int key, int scancode, int action, int mods) {
    for(const auto &[k, s] : Input::_KEY_SCAN_CODE_MAPPING) {
        if (s == key) {
            _input_state[k] = (action == GLFW_PRESS);
            return;
        }
    }
}

void Input::PollForEvents() {
    glfwPollEvents();
}

// If the hash table doesn't include the key asked for, it will be inserted
// here via the operator[], and its value automatically set to false (default value for bool)
bool Input::KeyPressed(Keys key, bool resetKeyIfPressed) {
    if (_input_state[key] && resetKeyIfPressed) {
        _input_state[key] = false;
        return true;
    }

    return _input_state[key];
}