#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <GL/glew.h>

/**
 *  Provides needed Vertex Formats representing the shape of our per-vertex data
 *  Also includes static, convenience methods for binding the correct vertex attribute
 *  format to the OpenGL pipeline
 */

namespace dss
{

struct PositionUV {
    glm::vec3 position;
    glm::vec2 uv;

    static constexpr size_t SizeInBytes() {
        return sizeof(position) + sizeof(uv);
    }

    // Convenience method to bind the vertex attribute layout to the pipeline
    static void SetVertexAttribPointers() {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (int)PositionUV::SizeInBytes(), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (int)PositionUV::SizeInBytes(), (void*)12);
    }

    PositionUV() = default;
    PositionUV(const glm::vec3 &position, const glm::vec2 &uv) : position(position), uv(uv) {}
};

}