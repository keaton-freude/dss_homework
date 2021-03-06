#pragma once

#include <unordered_map>
#include <memory>

#include "Window.h"

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
        // left-right arrows
        Left,
        Right
    };
    
private:
    // Hash table of the current state of keys. The value is set to 'True' if we consider
    // the button "pressed" which is a button which has been pressed & released
    std::unordered_map<Keys, bool> _input_state; 

    // A mapping between our defined Keys, to GLFW scan codes
    // Allows us to bridge GLFW state with our own internal state
    const static std::unordered_map<Keys, int> _KEY_SCAN_CODE_MAPPING;

    // The window we will receive state callbacks from
    std::shared_ptr<Window> _window;

    // Function GLFW will hit with keyboard updates
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void HandleKeyCallback(int key, int scancode, int action, int mods);

public:
    // Don't want to allow construction of an Input class without being given a GLFW window
    Input() = delete;

    // @window: the window to gather input from
    Input(std::shared_ptr<Window> window);

    // Call once per frame to update our state
    void PollForEvents();

    // User API for determining if a Key has been pressed during a frame
    // @resetKeyIfPressed: Whether we should immediately set the state of the key to
    // false if it was detected to be pressed. Avoids duplicate events
    // NOTE: Definitely hacky, but seems like too much work to design this in correctly
    bool KeyPressed(Keys key, bool resetKeyIfPressed = false);
};

}