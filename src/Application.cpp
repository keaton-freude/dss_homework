#include "Application.h"
#include "ShaderProgram.h"
#include "Utility.h"
#include "QuadMesh.h"
#include "MLBGameContent.h"

#include <GL/glew.h>
#include <stdexcept>
#include <iostream>
#include <fstream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <json.hpp>

#include <thread>


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
                    // Download a picture, for now just assume a 16:9 ratio.. probably use just 1440x720 as it'll look fine
                    // because it'll be pretty downscaled
                    httplib::SSLClient imageClient("securea.mlb.com");
                    
                    httplib::Headers headers = {
                        { "Content-Type", "application/octet-stream"}
                    };
                    auto imageResponse = imageClient.Get("/assets/images/7/9/6/280664796/cuts/1920x1080/cut.jpg", headers);
                    
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
        const auto& item = _contentQueue.front();
        std::cout << "Processing item with headline: " << item.title << std::endl;

        std::unique_ptr<ContentTile> contentTile = std::make_unique<ContentTile>(
            _texturedShader,
            glm::vec2(0.2f, 0.2f),
            _window->Width(),
            _window->Height()
        );

        _contentList->AddContentTile(std::move(contentTile));

        _contentQueue.pop();
    }
}

void DoFetchTileDetails(std::string date) {

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

        ProcessContentQueue();
    }
}