#pragma once

#include "Window.h"

#include <unordered_map>
#include <memory>

namespace dss
{

/**
 *  Provides a convenience class for querying current Input state.
 *  Scope-limited to just keyboard input and only a handful of keys.
 * 
 *  Normally this type of functionality would be more generic, such as supporting
 *  multiple backends for Input gathering.. mouse/joystick support, maybe some sort of
 *  Observable API. But for this application, keep it simple and focused 
 * 
 *  A goal here is to not leak GLFW-specific details
 */
class Input {
public:
    // The keys we are tracking
    enum class Keys {
        Escape,
        // left-right in vim keybindings
        L,
        H,
        // left-right arrows
        Left,
        Right,
        // left-right in WASD scheme
        A,
        D
    };
    
private:
    // Hash table of the current state of keys. The value is set to 'True' if we consider
    // the button "pressed" which is a button which has been pressed & released
    std::unordered_map<Keys, bool> _input_state = {
        {Keys::Escape, false},
        {Keys::L, false},
        {Keys::H, false},
        {Keys::Left, false},
        {Keys::Right, false},
        {Keys::A, false},
        {Keys::D, false}
    };

    // A mapping between our defined Keys, to GLFW scan codes
    // Allows us to bridge GLFW state with our own internal state
    const std::unordered_map<Keys, int> _KEY_SCAN_CODE_MAPPING = {
        {Keys::Escape, GLFW_KEY_ESCAPE},
        {Keys::L, GLFW_KEY_L},
        {Keys::H, GLFW_KEY_H},
        {Keys::Left, GLFW_KEY_LEFT},
        {Keys::Right, GLFW_KEY_RIGHT},
        {Keys::A, GLFW_KEY_A},
        {Keys::D, GLFW_KEY_D}
    };

    std::shared_ptr<Window> _window;


public:
    // Don't want to allow construction of an Input class without being given a GLFW window
    Input() = delete;

    // @window: the window to gather input from
    Input(std::shared_ptr<Window> window);

    // Call once per frame to update our state
    void PollForEvents();

    // User API for determining if a Key has been pressed during a frame
    bool KeyPressed(Keys key);
};

}