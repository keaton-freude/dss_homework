#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include <memory>

namespace dss
{

/**
 *  Draws a background image 
 */
class Background {
private:
    std::unique_ptr<IMesh> _mesh;

    IndexBuffer _indexBuffer;
    VertexBuffer _vertexBuffer;
public:
    // Will automatically configure itself (do we have dependencies on other stuff?)
    Background();

    void Draw();
};

}