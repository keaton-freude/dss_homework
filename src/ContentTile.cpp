#include "ContentTile.h"
#include "QuadMesh.h"
#include "Utility.h"

using namespace dss;

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, const std::string &title)
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture("textures/example_content.jpg"),
    _transform(),
    _title(title),
    _titleText(title, glm::vec2(_transform.translation.x, _transform.translation.y - 100.0f))
{
    QuadMesh mesh;
    _indexBuffer = IndexBuffer(&mesh);
    _vertexBuffer = VertexBuffer(&mesh);
}

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, const std::string &title, glm::vec2 position)
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture("textures/example_content.jpg"),
    _transform(glm::vec3(position.x, position.y, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
    _title(title),
    _titleText(title, glm::vec2(_transform.translation.x, _transform.translation.y - 100.0f))
{
}

void ContentTile::Draw(glm::mat4 view, glm::mat4 projection) {
    _indexBuffer.Bind();
    _vertexBuffer.Bind();
    _texture.Bind();
    PositionUV::SetVertexAttribPointers();
    _shader->Bind();
    auto model = _transform.GetModelMatrix();
    _shader->SetShaderUniform("Model", model);
    _shader->SetShaderUniform("View", view);
    _shader->SetShaderUniform("Projection", projection);

    glDrawElements(GL_TRIANGLES, _indexBuffer.GetNumFaces() * 3, GL_UNSIGNED_INT, 0);

    _titleText.Draw();
}

Transform ContentTile::GetTransform() const {
    return _transform;
}

void ContentTile::SetPosition(glm::vec3 position) {
    _transform.translation = position;

    _titleText.SetPosition(glm::vec2(position.x, position.y - _transform.scale.y));
}

void ContentTile::SetScale(glm::vec3 scale) {
    _transform.scale = scale;
}

void ContentTile::Resize(uint32_t width, uint32_t height) {
    _transform.scale.x = static_cast<float>(width);
    _transform.scale.y = static_cast<float>(height);

    _titleText.SetPosition(glm::vec2(_transform.translation.x, _transform.translation.y - _transform.scale.y));
}

void ContentTile::SetExpand(bool value) {
    std::cout << "Expand state? " << value << "\n";
    _expanded = value;
}