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

#include <memory>

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
    std::unique_ptr<Input> _input;
    std::shared_ptr<ShaderProgram> _texturedShader;
    Background _background;
    std::unique_ptr<ContentTileList> _contentList;
    // TESTING, NOT LONG TERM
    glm::mat4 _viewProjection;

    void CalculateViewProjection();


public:
    Application();
    ~Application() = default;

    // Starts a render loop, blocks until the application has closed for any reason
    void Run();
};

}