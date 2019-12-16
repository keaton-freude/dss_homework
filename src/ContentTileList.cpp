#include "ContentTileList.h"
#include <iostream>

#include <utility>

using namespace dss;

ContentTileList::ContentTileList(std::shared_ptr<ShaderProgram> shader, glm::vec2 position, uint32_t screenWidth, uint32_t screenHeight) 
    :   _shader(shader),
        _screenWidth(screenWidth),
        _screenHeight(screenHeight),
        _transform(std::make_shared<Transform>(glm::vec3(UnitToScreenSpaceWidth(_SPACE_BETWEEN_TILES), position.y, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)))
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
        rightEdgeOffset += UnitToScreenSpaceWidth(_SPACE_BETWEEN_TILES);

        _contentTiles[i]->SetXOffset(rightEdgeOffset);
    }
}

// Check two scenarios: selected tile is out of view to the left and selected tile is out of view to the right
void ContentTileList::BringSelectedTileIntoView() {
    const auto &expandedTile = _contentTiles[_selectedTileIndex];

    // Determine the position of the right-edge of the selected tile..
    const auto expandedRight = expandedTile->GetTransform().translation.x + expandedTile->GetTransform().scale.x;
    // Subtract off our own translation, then we can compare that to the window dimensions
    const auto adjusted = expandedRight + _transform->translation.x;

    // Don't compare to screen width directly, instead account for some buffer 
    const auto adjustedScreenWidth = _screenWidth - UnitToScreenSpaceWidth(_SPACE_BETWEEN_TILES);

    if (adjusted > adjustedScreenWidth) {
        const auto amount = adjusted - adjustedScreenWidth;

        _transform->translation.x -= amount;
    }

    const auto tileLeft = expandedTile->GetTransform().translation.x;

    const auto adjusted2 = tileLeft + _transform->translation.x;

    if (adjusted2 < UnitToScreenSpaceWidth(_SPACE_BETWEEN_TILES)) {
        const auto amount = UnitToScreenSpaceWidth(_SPACE_BETWEEN_TILES) - adjusted2;

        _transform->translation.x += amount;
    }
}

void ContentTileList::Draw(glm::mat4 view, glm::mat4 projection) {
    for (const auto& tile : _contentTiles) {
        tile->Draw(view, projection);
    }
}

void ContentTileList::Resize(uint32_t width, uint32_t height) {
    _screenWidth = width;
    _screenHeight = height;
}

void ContentTileList::AddContentTile(std::unique_ptr<ContentTile>&& tile) {
    // If this is the first tile, make sure its selected
    if (_contentTiles.size() == 0) {
        tile->SetExpand(true);
        tile->Resize(UnitToScreenSpaceWidth(0.2f * EXPAND_SCALE_FACTOR), UnitToScreenSpaceHeight(0.2f * EXPAND_SCALE_FACTOR));
    } else {
        tile->Resize(UnitToScreenSpaceWidth(0.2f), UnitToScreenSpaceHeight(0.2f));
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
    lastTile->Resize(UnitToScreenSpaceWidth(0.2f), UnitToScreenSpaceHeight(0.2f));
    currentTile->SetExpand(true);
    currentTile->Resize(UnitToScreenSpaceWidth(0.2f * EXPAND_SCALE_FACTOR), UnitToScreenSpaceHeight(0.2f * EXPAND_SCALE_FACTOR));
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