#include "ContentTileList.h"

#include <utility>

using namespace dss;

ContentTileList::ContentTileList(std::shared_ptr<ShaderProgram> shader, glm::vec2 position, uint32_t screenWidth, uint32_t screenHeight) 
    :   _shader(shader),
        _screenWidth(screenWidth),
        _screenHeight(screenHeight)
{
    uint32_t spaceBetween = (screenWidth * .2f) + 25;
}

void ContentTileList::Draw(glm::mat4 viewProjection) {
    // Just some silly hand-constructed ContentTiles to test
    for (const auto& tile : _contentTiles) {
        tile->Draw(viewProjection);
    }
}

void ContentTileList::Resize(uint32_t width, uint32_t height) {
    _screenWidth = width;
    _screenHeight = height;
}

void ContentTileList::AddContentTile(std::unique_ptr<ContentTile>&& tile) {
    // When this goes out of scope, the contentTilesMutex is released
    std::lock_guard<std::mutex> guard(_contentTilesMutex);
    _contentTiles.emplace_back(std::move(tile));
}