#include "ContentTile.h"
#include "QuadMesh.h"
#include "Utility.h"

using namespace dss;

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title, std::shared_ptr<Transform> parentTransform)
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture(std::move(textureData)),
    _transform(),
    _parentTransform(parentTransform),
    _title(title),
    _titleText(title, glm::vec2(_transform.translation.x, _transform.translation.y - 100.0f), _parentTransform)
{
    QuadMesh mesh;
    _indexBuffer = IndexBuffer(&mesh);
    _vertexBuffer = VertexBuffer(&mesh);
}

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, std::vector<unsigned char> &&textureData, const std::string &title, std::shared_ptr<Transform> parentTransform, glm::vec2 position)
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture(std::move(textureData)),
    _transform(glm::vec3(position.x, position.y, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
    _parentTransform(parentTransform),
    _title(title),
    _titleText(title, glm::vec2(_transform.translation.x, _transform.translation.y - 100.0f), _parentTransform)
{
}

void ContentTile::Draw(glm::mat4 view, glm::mat4 projection) {
    _indexBuffer.Bind();
    _vertexBuffer.Bind();
    _texture.Bind();
    PositionUV::SetVertexAttribPointers();
    _shader->Bind();
    auto model = _transform.GetModelMatrix();
    auto parentModel = _parentTransform->GetModelMatrix();
    auto combined = parentModel * model;

    _shader->SetShaderUniform("Model", combined);
    _shader->SetShaderUniform("View", view);
    _shader->SetShaderUniform("Projection", projection);

    glDrawElements(GL_TRIANGLES, _indexBuffer.GetNumFaces() * 3, GL_UNSIGNED_INT, 0);

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

void ContentTile::SetParentTransform(std::shared_ptr<Transform> transform) {
    _parentTransform = transform;
}

void ContentTile::UpdateTextPosition() {
    _titleText.SetPosition(glm::vec2(_transform.translation.x, -_transform.scale.y));
}