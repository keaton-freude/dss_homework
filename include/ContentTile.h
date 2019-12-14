#pragma once

#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include <glm/vec3.hpp>

#include <memory>


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
public:
    // Create a ContentTile at a default location
    ContentTile(std::shared_ptr<ShaderProgram> shader);

    // Create a ContentTile at a given location
    ContentTile(glm::vec3 position, std::shared_ptr<ShaderProgram> shader);
};

}