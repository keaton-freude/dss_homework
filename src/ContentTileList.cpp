#include "ContentTileList.h"

using namespace dss;

ContentTileList::ContentTileList(std::shared_ptr<ShaderProgram> shader, glm::vec2 position, uint32_t screenWidth, uint32_t screenHeight) 
    :   _shader(shader),
        _screenWidth(screenWidth),
        _screenHeight(screenHeight)
{
    uint32_t spaceBetween = (screenWidth * .2f) + 25;
    _contentTiles.emplace_back(std::make_unique<ContentTile>(shader, glm::vec2(.2f, .2f), screenWidth, screenHeight, position));
    _contentTiles.emplace_back(std::make_unique<ContentTile>(shader, glm::vec2(.2f, .2f), screenWidth, screenHeight, glm::vec2(position.x + (spaceBetween * 1), position.y)));
    _contentTiles.emplace_back(std::make_unique<ContentTile>(shader, glm::vec2(.2f, .2f), screenWidth, screenHeight, glm::vec2(position.x + (spaceBetween * 2), position.y)));
    _contentTiles.emplace_back(std::make_unique<ContentTile>(shader, glm::vec2(.2f, .2f), screenWidth, screenHeight, glm::vec2(position.x + (spaceBetween * 3), position.y)));
    _contentTiles.emplace_back(std::make_unique<ContentTile>(shader, glm::vec2(.2f, .2f), screenWidth, screenHeight, glm::vec2(position.x + (spaceBetween * 4), position.y)));

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