#include "Application.h"

using namespace dss;

// Currently a very opinionated default. Should expand the sort of
// configuration which can be passed to the Application
Application::Application() : _window(std::make_shared<Window>(1280, 720, "DSS Homework")), _input(std::make_unique<Input>(_window)) {
}

void Application::Run() {
    // Basic RenderLoop
    while (_window->Running()) {
        // Time calculation

        // Handle Input
        _input->PollForEvents();

        // Handle 'ESCAPE' being pressed, so we can tell the window it needs to close
        if (_input->KeyPressed(Input::Keys::Escape)) {
            // We'll tell the Window to close, but we want the windowing framework to decide
            // when we should actually close, so we'll wait until our Window tells us its
            // no longer running
            _window->Close();
        }

        // Draw the background
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(_window->GetWindow());

        // Draw the TitleRenderer
    }
}