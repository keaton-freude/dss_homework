#pragma once

#include <memory>
#include <string>
#include <list>
#include <functional>

#include "WindowResizeEvent.h"

// Forward declare is here, so we don't have the #include for GLFW in a public header
// Because GLFW ultimately includes gl.h, which breaks GLEW which needs to be included
// before gl.h is ever included. Thus, the "easy" way is to not include GLFW in headers
// otherwise you get annoying transitive includes causing the build to fail
struct GLFWwindow;

namespace dss
{
struct GlfwUserPointer;

// Wrap up a GLFWwindow. In a larger application where you might to support various windowing frameworks
// we'd abstract this concept out.
// I am also treating Window as a sort of container over GLFW. When this window is destroyed, so is anything
// GLFW related. Deleting your instance of a window will close the window, along with your opengl context
class Window {
private:
    // Raw pointer chosen because GLFW window pointer is not a typical pointer, we don't 'new' it
    // nor are we meant to do anything with it when we wish to stop using it. GLFW owns this pointer
    // we just have a view of it
    GLFWwindow *_window;
    // GLFW will hold onto the raw pointer and we'll delete it when the Window goes out of scope
    std::unique_ptr<GlfwUserPointer> _userPointer;

    uint16_t _width;
    uint16_t _height;

    // List of observers of resize events
    std::list<std::function<void(WindowResizeEvent)>> _resizeCallbacks{};

    // Called when the GLFW window is resized
    void WindowSizeCallback(GLFWwindow* window, int width, int height);
public:
    // Require user to specify arguments through other constructors
    Window() = delete;

    ~Window();

    // Create a window with the specified dimensions
    Window(uint16_t width, uint16_t height, const std::string &name);

    GLFWwindow *GetWindow();

    // Is the window currently in a Running state? I.e. its not preparing to, or already, exit
    bool Running() const;

    void SwapBuffers() const;

    // Tell the underlying window that it should close
    void Close() const;

    uint32_t Width() const;
    uint32_t Height() const;

    void RegisterResizeEventCallback(std::function<void(WindowResizeEvent)> callback);
};

}