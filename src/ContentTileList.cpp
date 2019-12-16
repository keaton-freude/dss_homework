#include <iostream>
#include <utility>

#include "ContentTileList.h"

using namespace dss;

ContentTileList::ContentTileList(std::shared_ptr<ShaderProgram> shader, glm::vec2 position, std::shared_ptr<CoordinateConverter> coordConverter) 
    :   _shader(shader),
        _transform(std::make_shared<Transform>(glm::vec3(coordConverter->UnitToScreenSpaceWidth(SPACE_BETWEEN_TILES), 
            position.y, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f))),
        _coordConverter(coordConverter)
{
}

void ContentTileList::ResizeElements() {
    // early out
    if (_contentTiles.size() == 0) {
        return;
    }

    _contentTiles[0]->SetXOffset(0.0f);

    // Now, each element is placed directly after the previous, with our pre-determined space-between value
    for(int i = 1; i < _contentTiles.size(); ++i) {
        auto previousTransform = _contentTiles[i - 1]->GetTransform();

        // Find the right-edge of the previous tile
        float rightEdgeOffset = previousTransform.translation.x + previousTransform.scale.x;
        // Add on our per-tile padding
        rightEdgeOffset += _coordConverter->UnitToScreenSpaceWidth(SPACE_BETWEEN_TILES);

        _contentTiles[i]->SetXOffset(rightEdgeOffset);
    }
}

// Check two scenarios: selected tile is out of view to the left and selected tile is out of view to the right
void ContentTileList::BringSelectedTileIntoView() {
    // Handle the case where the selected tile is past the edge of the right side of the window
    const auto &expandedTile = _contentTiles[_selectedTileIndex];

    // Determine the position of the right-edge of the selected tile..
    const auto expandedRight = expandedTile->GetTransform().translation.x + expandedTile->GetTransform().scale.x;
    // Subtract off our own translation, then we can compare that to the window dimensions
    const auto adjustedRight = expandedRight + _transform->translation.x;

    // Don't compare to screen width directly, instead account for some buffer space
    const auto adjustedScreenWidth = _coordConverter->ScreenWidth() - _coordConverter->UnitToScreenSpaceWidth(SPACE_BETWEEN_TILES);

    if (adjustedRight > adjustedScreenWidth) {
        const auto amount = adjustedRight - adjustedScreenWidth;

        _transform->translation.x -= amount;
    }

    // Handle the case where the selected tile is past the edge of the left side of the window
    const auto expandedLeft = expandedTile->GetTransform().translation.x;

    const auto adjustedLeft = expandedLeft + _transform->translation.x;

    if (adjustedLeft < _coordConverter->UnitToScreenSpaceWidth(SPACE_BETWEEN_TILES)) {
        const auto amount = _coordConverter->UnitToScreenSpaceWidth(SPACE_BETWEEN_TILES) - adjustedLeft;

        _transform->translation.x += amount;
    }
}

void ContentTileList::Draw(glm::mat4 view, glm::mat4 projection) {
    for (const auto& tile : _contentTiles) {
        tile->Draw(view, projection);
    }
}

void ContentTileList::AddContentTile(std::unique_ptr<ContentTile>&& tile) {
    // If this is the first tile, make sure its selected
    if (_contentTiles.size() == 0) {
        tile->SetExpand(true);
        tile->Resize(_coordConverter->UnitToScreenSpaceWidth(TILE_SCALE * EXPAND_SCALE_FACTOR), 
            _coordConverter->UnitToScreenSpaceHeight(TILE_SCALE * EXPAND_SCALE_FACTOR));
    } else {
        tile->Resize(_coordConverter->UnitToScreenSpaceWidth(TILE_SCALE), _coordConverter->UnitToScreenSpaceHeight(TILE_SCALE));
    }

    tile->SetParentTransform(_transform);

    // When this goes out of scope, the contentTilesMutex is released
    std::lock_guard<std::mutex> guard(_contentTilesMutex);
    _contentTiles.emplace_back(std::move(tile));

    ResizeElements();
}

void ContentTileList::AddContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title) {
    AddContentTile(std::make_unique<ContentTile>(
        shader,
        std::move(textureData),
        title,
        _transform
    ));
}

void ContentTileList::ExpandTile(size_t last, size_t current) {
    auto &lastTile = _contentTiles[last];
    auto &currentTile = _contentTiles[current];

    lastTile->SetExpand(false);
    lastTile->Resize(_coordConverter->UnitToScreenSpaceWidth(TILE_SCALE), _coordConverter->UnitToScreenSpaceHeight(TILE_SCALE));
    currentTile->SetExpand(true);
    currentTile->Resize(_coordConverter->UnitToScreenSpaceWidth(TILE_SCALE * EXPAND_SCALE_FACTOR), 
        _coordConverter->UnitToScreenSpaceHeight(TILE_SCALE * EXPAND_SCALE_FACTOR));
}

void ContentTileList::SelectNextTile() {
    // Check that we don't go out of bounds
    if (_contentTiles.size() == 0 || _selectedTileIndex >= _contentTiles.size() - 1) {
        return;
    }

    ExpandTile(_selectedTileIndex, _selectedTileIndex + 1);
    _selectedTileIndex++;
    ResizeElements();

    BringSelectedTileIntoView();
}

void ContentTileList::SelectPreviousTile() {
    // Check that we don't go out of bounds
    if (_contentTiles.size() == 0 || _selectedTileIndex == 0) {
        return;
    }

    ExpandTile(_selectedTileIndex, _selectedTileIndex - 1);
    _selectedTileIndex--;
    ResizeElements();

    BringSelectedTileIntoView();
}