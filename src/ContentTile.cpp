#include "ContentTile.h"
#include "QuadMesh.h"
#include "Utility.h"

using namespace dss;

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title, std::shared_ptr<Transform> parentTransform)
:   TexturedDrawable(&QuadMesh(), std::move(textureData), shader),
    _transform(),
    _parentTransform(parentTransform),
    _titleText(title, glm::vec2(_transform.translation.x, _transform.translation.y - 100.0f), _parentTransform)
{
}

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title, std::shared_ptr<Transform> parentTransform, glm::vec2 position)
:   TexturedDrawable(&QuadMesh(), std::move(textureData), shader),
    _transform(glm::vec3(position.x, position.y, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
    _parentTransform(parentTransform),
    _titleText(title, glm::vec2(_transform.translation.x, _transform.translation.y - 100.0f), _parentTransform)
{
}

void ContentTile::Draw(const glm::mat4 &view, const glm::mat4 &projection) {
    // Add on the parent transforms translation
    // Skip rotation & scale as we don't want the parent to affect those
    auto combinedTransform = _transform;
    combinedTransform.translation += _parentTransform->translation; 

    TexturedDrawable::DoDraw(combinedTransform, view, projection);

    if (_expanded) {
        _titleText.Draw();
    }
}

Transform ContentTile::GetTransform() const {
    return _transform;
}

// Adjust the x-offset of the ContentTile's local transform
void ContentTile::SetXOffset(float offset) {
    _transform.translation.x = offset;

    // Position of text needs adjusted
    UpdateTextPosition();
}

// Change the dimensions of the ContentTile
void ContentTile::Resize(uint32_t width, uint32_t height) {
    _transform.scale.x = static_cast<float>(width);
    _transform.scale.y = static_cast<float>(height);

    // Position of text needs adjusted
    UpdateTextPosition();
}

void ContentTile::SetExpand(bool value) {
    _expanded = value;
}

bool ContentTile::IsExpanded() const {
    return _expanded;
}

void ContentTile::SetParentTransform(std::shared_ptr<Transform> transform) {
    _parentTransform = transform;
}

void ContentTile::UpdateTextPosition() {
    _titleText.SetPosition(glm::vec2(_transform.translation.x, -_transform.scale.y));
}