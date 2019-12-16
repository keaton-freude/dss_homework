#pragma once

#include "Window.h"
#include "Input.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "glm/mat4x4.hpp"
#include "Background.h"
#include "ContentTile.h"
#include "ContentTileList.h"
#include "ContentTileData.h"
#include "Text.h"
#include "CoordinateConverter.h"
#include "MLBStatsFetcher.h"

#include <memory>
#include <thread>
#include <list>
#include <queue>
#include <mutex>

namespace dss
{

/**
 *  dss::Application will implement the main business logic of the application
 *  which also includes the RenderLoop, along with reacting to input and communicating
 *  with sub-components.
 *  
 *  The dss::Application class will take on a fair amount of responsibility, its ripe
 *  for abstraction and breaking components out. But we'll start with a lot of stuff being
 *  in here and things will be extracted/modularized as it becomes more apparent
 * 
 *  A key requirement here is we should, ideally, see _no_ GLFW or OpenGL stuff. We should
 *  work over top of our own abstractions. Given time, I'd extend that over the HTTP library
 *  and the JSON library.
 * 
 *  TODO: Re-evaluate above
 * 
 *  NOTE: Things I'd normally split out: RenderLoop abstraction <TODO> add more 
 */
class Application {
private:
    // The Window this application uses for Input & Rendering
    std::shared_ptr<Window> _window;

    // Input events for the window can be queried from this
    std::unique_ptr<Input> _input;

    // The shader we will use to render the background and content
    std::shared_ptr<ShaderProgram> _texturedShader;

    // Provides capability for drawing a full-window texture
    Background _background;

    // The content list we will render based on external data from MLB stats API
    std::unique_ptr<ContentTileList> _contentList;

    // Our view matrix
    glm::mat4 _view;

    // Our projection matrix: Will be an ortho projection with dimensions matching
    // our screen
    glm::mat4 _projection;

    // Provides a way to reliably convert coordinates between unit and screen space
    std::shared_ptr<CoordinateConverter> _coordConverter;

    // Fetch stats from MLB API and notify observers
    MLBStatsFetcher _statsFetcher;

    void CalculateViewProjection();

    // Background threads will push new content tile data into the queue
    // The main thread will pull items off the queue and create the actual Content Tiles
    std::mutex _contentQueueLock;
    std::queue<ContentTileData> _contentQueue;

    void ProcessContentQueue();

public:
    Application();

    // Starts a render loop, blocks until the application has closed for any reason
    void Run();
};

}