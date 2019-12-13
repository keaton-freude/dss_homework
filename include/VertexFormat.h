#pragma once

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

// TODO: For testing, probably delete this before submitting
// Represents a 3d position, and a corresponding 4-element color
struct PositionColor {
    glm::vec3 position;
    glm::vec4 color;

    static constexpr size_t SizeInBytes() {
        return sizeof(position) + sizeof(color);
    }

    // Convenience method to bind the vertex attribute layout to the pipeline
    static void SetVertexAttribPointers() {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (int)PositionColor::SizeInBytes(), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (int)PositionColor::SizeInBytes(), (void*)12);
    }

    PositionColor() = default;
    PositionColor(const glm::vec3 &position, const glm::vec4 &color) : position(position), color(color) {}
};

}