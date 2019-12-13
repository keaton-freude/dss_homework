#include "Input.h"

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
}

void Input::PollForEvents() {
    glfwPollEvents();
    // Walk the key -> GLFW scan code mapping, querying the state of each
    // and updating our internal state
    for(const auto& [key, scan_code] : Input::_KEY_SCAN_CODE_MAPPING) {
        auto state = glfwGetKey(_window->GetWindow(), scan_code);
        _input_state[key] = (state == GLFW_PRESS);
    }
}

// If the hash table doesn't include the key asked for, it will be inserted
// here via the operator[], and its value automatically set to false (default value for bool)
bool Input::KeyPressed(Keys key) {
    return _input_state[key];
}