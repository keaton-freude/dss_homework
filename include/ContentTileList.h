#pragma once

#include <vector>

#include "ContentTile.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"

namespace dss
{

/**
 *  A ContentTileList refers to an entire collection of ContentTiles rendered in
 *  a horizonital list
 * 
 *  We can specify a gap between titles, and the transform of the list itself
 * 
 *  We can render an arbitrary amount of content tiles. Tiles are added _externally_
 *  and adding a Content Tile is thread-safe. The imagined use-case is some external thread
 *  is constructing Content Tiles and adding them to the list. As items are added
 *  we will begin rendering them. 
 */

class ContentTileList {
private:
    std::vector<std::unique_ptr<ContentTile>> _contentTiles;
    std::shared_ptr<ShaderProgram> _shader;

    uint32_t _screenWidth;
    uint32_t _screenHeight;
public:
    // TODO: Probably delete
    ContentTileList();

    ContentTileList(std::shared_ptr<ShaderProgram> shader, glm::vec2 position, uint32_t screenWidth, uint32_t screenHeight);

    void Draw(glm::mat4 viewProjection);

    void Resize(uint32_t width, uint32_t height);
};

}