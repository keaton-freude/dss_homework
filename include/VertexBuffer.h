#pragma once

#include "IMesh.h"

#include <stdint.h>

namespace dss
{

/**
 *  Wraps an OpenGL Vertex Buffer.
 * 
 *  Takes an IMesh which provides an accessor to the vertex data
 *  along with how much data there is
 * 
 *  Provides a Bind function to bind it to the pipeline
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