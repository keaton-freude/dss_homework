#pragma once

#include "Input.h"
#include "Window.h"

namespace dss
{

/**
 *  Unfortunately GLFW only lets us set a single user pointer (so we can reference instances
 *  of our class during GLFW callbacks), so this struct just wraps up all of the pointers
 *  needed by various components to handle GLFW callbacks 
 */
struct GlfwUserPointer {
    // Non-owning Raw pointers
    Input *input;
    Window *window;
};

}