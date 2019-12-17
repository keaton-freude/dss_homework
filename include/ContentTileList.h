#pragma once

#include <vector>
#include <mutex>

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"

#include "ContentTile.h"
#include "CoordinateConverter.h"
#include "ShaderProgram.h"
#include "Transform.h"


namespace dss
{

/**
 *  A ContentTileList refers to an entire collection of ContentTiles rendered in
 *  a horizonital list
 * 
 *  We can specify a gap between titles, and the transform of the list itself.
 *  The gap also applies to the beginning and end of the list
 * 
 *  We can render an arbitrary amount of content tiles. Tiles are added _externally_
 *  and adding a Content Tile is thread-safe. The imagined use-case is some external thread
 *  is constructing Content Tiles and adding them to the list. As items are added
 *  we will begin rendering them. 
 */

class ContentTileList {
private:
    // 50% larger for expanded tiles
    const float EXPAND_SCALE_FACTOR = 1.50f;
    const float SPACE_BETWEEN_TILES = 0.02f;

    // How large each tile should be in unit-coords
    const float TILE_SCALE = 0.2f;

    // The tiles we are managing
    std::vector<std::unique_ptr<ContentTile>> _contentTiles;

    // Make adding tiles thread-safe with this per-instance mutex
    std::mutex _contentTilesMutex;

    // Shader to draw tiles with
    std::shared_ptr<ShaderProgram> _shader;

    // Local transform of the ContentTileList
    std::shared_ptr<Transform> _transform;

    // Provides a method for converting between unit and screen space coordinates
    // along two axis (x & y)
    std::shared_ptr<CoordinateConverter> _coordConverter;

    // Which tile in our vector is selected
    // TODO Replace with iterator?
    size_t _selectedTileIndex = 0;

    // This method will add a tile, to the contentTiles list
    // This method is thread-safe
    void AddContentTile(std::unique_ptr<ContentTile> &&tile);

    // Expand @current Tile, un-expand @last tile
    void ExpandTile(size_t last, size_t current);

    /// -- Content layout functionality --
    /// Purpose-built and looking back should've wrote a content layout manager to handle layouts
    /// instead of purpose-building a layout manager for just Content Tiles

    // Take into account the current tiles, and whether one is selected and update their transforms
    void ResizeElements();

    // Ensure the selected tile is visible and padding rules are observed
    void BringSelectedTileIntoView();
    /// ----------------------------------

public:
    ContentTileList() = delete;

    // Construct a ContentTileList --
    // We require the shader we will be drawing with, a local-position for our transform (x,y only) and a method of converting
    // coordinates between unit and screen space
    ContentTileList(std::shared_ptr<ShaderProgram> shader, glm::vec2 position, std::shared_ptr<CoordinateConverter> coordConverter);

    // Render the entire ContentList
    void Draw(glm::mat4 view, glm::mat4 projection);

    // Add a content tile to the list --
    // Safe to call this method from multiple threads
    void AddContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title, const std::string &blurb);

    // Select the next tile in the list
    void SelectNextTile();

    // Select the previous tile in the list
    void SelectPreviousTile();

    // Recalculate element layout positioning
    void RespondToResize();
};

}