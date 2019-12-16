#pragma once

#include <string>
#include <memory>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"
#include "Texture.h"
#include "TexturedDrawable.h"
#include "Transform.h"
#include "Text.h"


namespace dss
{

/**
 *  A ContentTile represents a tile in the app which has an image of some content
 *  and displays text when it is selected 
 * 
 *  ContentTiles shouldn't place themselves, but rather be placed explicitly
 * 
 */
class ContentTile : public TexturedDrawable<PositionUV> {
private:
    // Local transform of the Content Tile
    Transform _transform;

    // The transform of our parent (ContentTileList), this allows us to easily
    // keep tiles grouped together and moved together, relative to the parent
    std::shared_ptr<Transform> _parentTransform;

    // Title text
    Text _titleText;

    // Whether this tile is selected and should show extra info & expand itself 
    bool _expanded = false;

    // Call this when the size of the content tile changes, or its local-transform is changed
    void UpdateTextPosition();

public:
    // Create a ContentTile at a default location
    ContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title, std::shared_ptr<Transform> parentTransform);

    // Create a ContentTile at a given location
    ContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title, std::shared_ptr<Transform> parentTransform, glm::vec2 position);

    virtual void Draw(const glm::mat4 &view, const glm::mat4 &projection) override;

    Transform GetTransform() const;

    void SetXOffset(float offset);

    void SetScale(glm::vec3 scale);

    void Resize(uint32_t width, uint32_t height);

    void SetExpand(bool value);

    bool IsExpanded() const;

    void SetParentTransform(std::shared_ptr<Transform> transform);
};

}