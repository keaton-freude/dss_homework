#pragma once

#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>

// TODO TODO TODO
// Extract the drawing part _out_ of this and the background class. they are almost exactly the same
// think something like DrawableNode in Nimble

namespace dss
{

/**
 *  A ContentTile represents a tile in the app which has an image of some content
 *  and displays text when it is selected 
 * 
 *  ContentTiles shouldn't place themselves, but rather be placed explicitly
 * 
 *  ContentTiles will load a default texture, until a Texture is set via the
 *  SetTexture method
 */
class ContentTile {
private:
    std::shared_ptr<ShaderProgram> _shader;
    IndexBuffer _indexBuffer;
    VertexBuffer _vertexBuffer;
    Texture _texture;
    Transform _transform;

    // Pixel values of width and height
    uint32_t _width;
    uint32_t _height;

    void CalculateSize();
public:
    // Create a ContentTile at a default location
    ContentTile(std::shared_ptr<ShaderProgram> shader);

    // Create a ContentTile at a given location
    ContentTile(std::shared_ptr<ShaderProgram> shader, glm::vec3 position, uint32_t width, uint32_t height);

    void Draw(glm::mat4 viewProjection);

    void Resize(uint32_t width, uint32_t height) {
        _width = width;
        _height = height;

        CalculateSize();
    }
};

}