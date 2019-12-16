#pragma once

#include <vector>

#include "ContentTile.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"

#include <mutex>

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
    std::mutex _contentTilesMutex;
    std::shared_ptr<ShaderProgram> _shader;

    const float _SPACE_BETWEEN_TILES = 0.02f;
    uint32_t _screenWidth;
    uint32_t _screenHeight;
    std::shared_ptr<Transform> _transform;

    // Which tile in our vector is selected
    size_t _selectedTileIndex = 0;

    // 25% larger for expanded tiles
    const float EXPAND_SCALE_FACTOR = 1.25f;


    float UnitToScreenSpaceWidth(float unitAmount) {
        return _screenWidth * unitAmount;
    }

    float UnitToScreenSpaceHeight(float unitAmount) {
        return _screenHeight * unitAmount;
    }

    float ScreenSpaceToUnitWidth(float screenAmount) {
        return screenAmount / _screenWidth;
    }

    float ScreenSpaceToUnitHeight(float screenAmount) {
        return screenAmount / _screenHeight;
    }

    // This method will add a tile, to the contentTiles list
    // This method is thread-safe
    void AddContentTile(std::unique_ptr<ContentTile> &&tile);

    // Take into account the current tiles, and whether one is selected and update their transforms
    void ResizeElements();
    // last is the previously expanded tile, current is the tile to be expanded
    void ExpandTile(size_t last, size_t current);

    void BringSelectedTileIntoView();
public:
    // TODO: Probably delete
    ContentTileList();

    ContentTileList(std::shared_ptr<ShaderProgram> shader, glm::vec2 position, uint32_t screenWidth, uint32_t screenHeight);

    void Draw(glm::mat4 view, glm::mat4 projection);

    void Resize(uint32_t width, uint32_t height);

    void AddContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title);

    void SelectNextTile();

    void SelectPreviousTile();
};

}