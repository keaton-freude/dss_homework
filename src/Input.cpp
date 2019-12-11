#include "Input.h"

using namespace dss;

Input::Input(std::shared_ptr<Window> window) : _window(window) {

}

void Input::PollForEvents() {
    glfwPollEvents();
    // Walk the key -> GLFW scan code mapping, querying the state of each
    // and updating our internal state
    for(const auto& [key, scan_code] : _KEY_SCAN_CODE_MAPPING) {
        auto state = glfwGetKey(_window->GetWindow(), scan_code);
        _input_state[key] = (state == GLFW_PRESS);
    }
}

bool Input::KeyPressed(Keys key) {
    return _input_state[key];
}