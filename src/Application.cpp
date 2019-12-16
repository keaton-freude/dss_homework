#include "Application.h"
#include "ShaderProgram.h"
#include "Utility.h"
#include "QuadMesh.h"
#include "MLBGameContent.h"
#include "Url.h"

#include <GL/glew.h>
#include <stdexcept>
#include <iostream>
#include <fstream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <json.hpp>

#include <thread>


#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace dss;

// Currently a very opinionated default. Should expand the sort of
// configuration which can be passed to the Application
Application::Application() 
    :   _window(std::make_shared<Window>(1920, 1080, "DSS Homework")), 
        _input(std::make_unique<Input>(_window)),
        _texturedShader(std::make_shared<ShaderProgram>(FileReadAllText(GetPathToResource("shaders/textured.vert")),
            FileReadAllText(GetPathToResource("shaders/textured.frag")))),
        _background(_window, _texturedShader),
        _coordConverter(std::make_shared<CoordinateConverter>(_window))
{


    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(_window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
    
    _background.SetSize(_window->Width(), _window->Height());
    CalculateViewProjection();

    _contentList = std::make_unique<ContentTileList>(_texturedShader, glm::vec2(0.0f, 540.f), _coordConverter);

    // Need to relay window dimension changes to components which need it
    _window->RegisterResizeEventCallback([this](WindowResizeEvent event) {
        this->_background.SetSize(event.newWidth, event.newHeight);
        this->CalculateViewProjection();
    });

    _threads.push_back(std::thread([this](){
        httplib::Client client("statsapi.mlb.com");

        auto res = client.Get("/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1");
        if (res && res->status == 200) {
            // good to go
            std::cout << "Got response and its good!" << std::endl;

            // Decode the body into a JSON object
            auto parsed = nlohmann::json::parse(res->body);
            auto data = parsed.get<MLBStats>();
            std::cout << "Got " << data.totalGames << " total games of data" << std::endl;

            {
                for(int i = 0; i < data.totalGames; ++i) {
                    // Download a picture, for now just assume a 16:9 ratio.. probably use just 1280x720 as it'll look fine
                    // because it'll be pretty downscaled

                    // Take the image source and break it out into the host and endpoint
                    auto url = Url(data.dates[0].games[i].content.editorial.recap.home.photo.cuts["1280x720"].src);

                    httplib::SSLClient imageClient(url.Host().c_str());
                    
                    httplib::Headers headers = {
                        { "Content-Type", "application/octet-stream"}
                    };


                    auto imageResponse = imageClient.Get(url.Path().c_str(), headers);
                    
                    if (imageResponse && imageResponse->status == 200) {
                        std::cout << "Got an image!" << std::endl;
                        std::lock_guard(this->_contentQueueLock);
                        auto &game = data.dates[0].games[i];
                        std::vector<unsigned char> textureData(imageResponse->body.begin(), imageResponse->body.end());
                        this->_contentQueue.emplace(ContentTileData(game.content.editorial.recap.home.headline, std::move(textureData)));
                    }

                    {
                    }
                }
            }
        } else {
            std::abort();
        }
    }));
}

Application::~Application() {
    for(auto& t : _threads) {
        t.join();
    }
}

void Application::ProcessContentQueue() {
    const std::lock_guard<std::mutex> lock(_contentQueueLock);
    if (_contentQueue.empty()) {
        return;
    }

    while (!_contentQueue.empty()) {
        auto& item = _contentQueue.front();
        std::cout << "Processing item with headline: " << item.title << std::endl;
        _contentList->AddContentTile(_texturedShader, std::move(item.textureData), item.title);

        _contentQueue.pop();
    }
}

void DoFetchTileDetails(std::string date) {

}

void Application::CalculateViewProjection() {
    _projection = glm::ortho(0.f, static_cast<float>(_window->Width()), 0.f, static_cast<float>(_window->Height()), -100.f, 100.f);
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

        // Draw the background
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
        ImGui::SetNextWindowSize(ImVec2(_window->Width(), _window->Height()));
        ImGui::Begin("Hello, world!", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);

        _background.Draw(_view, _projection);
        _contentList->Draw(_view, _projection);

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        _window->SwapBuffers();

        ProcessContentQueue();
    }
}