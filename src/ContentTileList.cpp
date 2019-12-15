#include "ContentTileList.h"

#include <utility>

using namespace dss;

ContentTileList::ContentTileList(std::shared_ptr<ShaderProgram> shader, glm::vec2 position, uint32_t screenWidth, uint32_t screenHeight) 
    :   _shader(shader),
        _transform(glm::vec3(0.0f, UnitToScreenSpaceWidth(0.5f), 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
        _screenWidth(screenWidth),
        _screenHeight(screenHeight)
{
    uint32_t spaceBetween = (screenWidth * .2f) + 25;
}

void ContentTileList::ResizeElements() {
    // early out
    if (_contentTiles.size() == 0) {
        return;
    }

    // The first element is always offset from the left-most portion of the screen by the "SPACE_BETWEEN_TILES" constant
    _contentTiles[0]->SetPosition(glm::vec3(UnitToScreenSpaceWidth(_SPACE_BETWEEN_TILES), 500.0f, 0.0f));

    // Now, each element is placed directly after the previous, with our pre-determined space-between value
    for(int i = 1; i < _contentTiles.size(); ++i) {
        auto transform = _contentTiles[i - 1]->GetTransform();
        float amt1 = (UnitToScreenSpaceWidth(0.2f + _SPACE_BETWEEN_TILES));
        float amt2 = ScreenSpaceToUnitWidth(transform.scale.x) / 0.2f;
        transform.translation.x += amt1 * amt2;
        transform.translation.y = 500.0f;
        _contentTiles[i]->SetPosition(transform.translation);
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
    tile->Resize(UnitToScreenSpaceWidth(0.2f), UnitToScreenSpaceHeight(0.2f));

    // When this goes out of scope, the contentTilesMutex is released
    std::lock_guard<std::mutex> guard(_contentTilesMutex);
    _contentTiles.emplace_back(std::move(tile));

    ResizeElements();
}