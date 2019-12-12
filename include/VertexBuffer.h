#pragma once

#include "IMesh.h"

namespace dss
{

/**
 *  Wraps an OpenGL Vertex Buffer
 *  TODO: ADD DOCS?
 */
class VertexBuffer {
private:
    uint32_t _vertexBufferHandle;
public:
    VertexBuffer() = delete;

    VertexBuffer(IMesh *mesh);

    void Bind() const;
};

}