#include "Application.h"
#include "ShaderProgram.h"
#include "Utility.h"
#include "QuadMesh.h"

#include <GL/glew.h>
#include <stdexcept>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace dss;

// Currently a very opinionated default. Should expand the sort of
// configuration which can be passed to the Application
Application::Application() 
    :   _window(std::make_shared<Window>(1920, 1080, "DSS Homework")), 
        _input(std::make_unique<Input>(_window)),
        _texturedShader(std::make_shared<ShaderProgram>(FileReadAllText(GetPathToResource("shaders/textured.vert")),
            FileReadAllText(GetPathToResource("shaders/textured.frag")))),
        _background(_window, _texturedShader) {
    // Now that the Window is created, our OpenGL context is created and set, safe to initialize glew
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to Initialize GLEW!");
    }

    auto ortho = glm::ortho(0.f, 1920.f, 0.f, 1080.f, -100.f, 100.f);
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));

    _viewProjection = ortho * view;

    _contentTile = std::make_unique<ContentTile>(_texturedShader, glm::vec3(100.0f, 100.0f, 0.0f), 16 * 40, 9 * 40);
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

        _background.Draw(_viewProjection);
        _contentTile->Draw(_viewProjection);
        _window->SwapBuffers();
        // Draw the TitleRenderer
    }
}