#include <thread>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <json.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Application.h"
#include "ShaderProgram.h"
#include "Utility.h"
#include "QuadMesh.h"
#include "MLBGameContent.h"

using namespace dss;

// Currently a very opinionated default. Should expand the sort of
// configuration which can be passed to the Application
Application::Application(std::optional<Proxy> proxy) 
    :   _window(std::make_shared<Window>(1920, 1080, "DSS Homework")), 
        _input(std::make_unique<Input>(_window)),
        _texturedShader(std::make_shared<ShaderProgram>(FileReadAllText(GetPathToResource("shaders/textured.vert")),
            FileReadAllText(GetPathToResource("shaders/textured.frag")))),
        _background(_texturedShader),
        _coordConverter(std::make_shared<CoordinateConverter>(_window)),
        _statsFetcher(proxy)
{
    _statsFetcher.AddObserver([this](std::string title, std::string blurb, std::vector<unsigned char>&& textureData){
        std::lock_guard<std::mutex> lock(this->_contentQueueLock);
        _contentQueue.push(ContentTileData(title, blurb, std::move(textureData)));
    });

    _statsFetcher.StartForDate("2018-06-10");

    // Required ImGui setup for our combination of Gfx API & Windowing API
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(_window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // Load ImGUI Fonts. We want two different sized fonts, using the default
    // font-face
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(GetPathToResource("fonts/roboto-regular.ttf").string().c_str(), 28.f);
    io.Fonts->AddFontFromFileTTF(GetPathToResource("fonts/roboto-regular.ttf").string().c_str(), 16.f);
    
    _background.SetSize(_window->Width(), _window->Height());
    CalculateViewProjection();

    _contentList = std::make_unique<ContentTileList>(_texturedShader, glm::vec2(0.0f, 0.5f), _coordConverter);

    // Need to relay window dimension changes to components which need it
    _window->RegisterResizeEventCallback([this](WindowResizeEvent event) {
        this->_background.SetSize(event.newWidth, event.newHeight);
        this->CalculateViewProjection();
        this->_contentList->RespondToResize();
    });
}

Application::~Application() {
	// cleanup ImGui
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void Application::ProcessContentQueue() {
    const std::lock_guard<std::mutex> lock(_contentQueueLock);
    if (_contentQueue.empty()) {
        return;
    }

    while (!_contentQueue.empty()) {
        auto& item = _contentQueue.front();
        std::cout << "Processing item with headline: " << item.title << std::endl;
        _contentList->AddContentTile(_texturedShader, std::move(item.textureData), item.title, item.blurb);

        _contentQueue.pop();
    }
}

void Application::CalculateViewProjection() {
    // Arbitrary Near & Far
    static const float NEAR_DISTANCE = -100.0f;
    static const float FAR_DISTANCE = 100.0f;
    _projection = glm::ortho(0.f, static_cast<float>(_window->Width()), 0.f, static_cast<float>(_window->Height()), NEAR_DISTANCE, FAR_DISTANCE);
    _view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
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

        if (_input->KeyPressed(Input::Keys::Left, true)) {
            _contentList->SelectPreviousTile();
        }

        if (_input->KeyPressed(Input::Keys::Right, true)) {
            _contentList->SelectNextTile();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // Required ImGUI calls to keep things moving
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Create a full-screen, completely transparent window to house our text
        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(_window->Width()), static_cast<float>(_window->Height())));
        ImGui::Begin("Hello, world!", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);

        // Draw our stuff. Background and content list
        _background.Draw(_view, _projection);
        _contentList->Draw(_view, _projection);

        // Process any items which may be in the content queue
        ProcessContentQueue();

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present frame
        _window->SwapBuffers();

    }
}