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
    //_contentTile = std::make_unique<ContentTile>(_texturedShader, glm::vec2(0.22f, 0.22f), _window->Width(), _window->Height(), glm::vec3(25.0f, 500.0f, 0.0f));
    _contentList = std::make_unique<ContentTileList>(_texturedShader, glm::vec2(25.f, 500.f), _window->Width(), _window->Height());

    // Resize the background if the window changes
    _window->RegisterResizeEventCallback([this](WindowResizeEvent event) {
        this->_background.SetSize(event.newWidth, event.newHeight);
        this->CalculateViewProjection();
        this->_contentList->Resize(event.newWidth, event.newHeight);

        // Hmm this doesn't _exactly_ belong here.. Don't want opengl concepts flooding app code..
        glViewport(0, 0, (int)event.newWidth, (int)event.newHeight);
    });

    glm::vec2 startPosition = glm::vec2(25.f, 500.f);
    uint32_t spaceBetween = (_window->Width() * .2) + 25;
    _contentList->AddContentTile(std::make_unique<ContentTile>(_texturedShader, glm::vec2(.2f, .2f), _window->Width(), _window->Height(), startPosition));
    _contentList->AddContentTile(std::make_unique<ContentTile>(_texturedShader, glm::vec2(.2f, .2f), _window->Width(), _window->Height(), glm::vec2(startPosition.x + (spaceBetween * 1), startPosition.y)));
    _contentList->AddContentTile(std::make_unique<ContentTile>(_texturedShader, glm::vec2(.2f, .2f), _window->Width(), _window->Height(), glm::vec2(startPosition.x + (spaceBetween * 2), startPosition.y)));
    _contentList->AddContentTile(std::make_unique<ContentTile>(_texturedShader, glm::vec2(.2f, .2f), _window->Width(), _window->Height(), glm::vec2(startPosition.x + (spaceBetween * 3), startPosition.y)));
    _contentList->AddContentTile(std::make_unique<ContentTile>(_texturedShader, glm::vec2(.2f, .2f), _window->Width(), _window->Height(), glm::vec2(startPosition.x + (spaceBetween * 4), startPosition.y)));
}

void Application::CalculateViewProjection() {
    auto ortho = glm::ortho(0.f, static_cast<float>(_window->Width()), 0.f, static_cast<float>(_window->Height()), -100.f, 100.f);
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));

    _viewProjection = ortho * view;
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
        _contentList->Draw(_viewProjection);
        _window->SwapBuffers();
        // Draw the TitleRenderer
    }
}