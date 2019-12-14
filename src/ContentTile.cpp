#include "ContentTile.h"
#include "QuadMesh.h"
#include "Utility.h"

using namespace dss;

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, glm::vec2 size, uint32_t screenWidth, uint32_t screenHeight)
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture("textures/example_content.jpg"),
    _transform(),
    _size(size),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight)
{
    QuadMesh mesh;
    _indexBuffer = IndexBuffer(&mesh);
    _vertexBuffer = VertexBuffer(&mesh);

    CalculateSize();
}

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, glm::vec2 size, uint32_t screenWidth, uint32_t screenHeight, glm::vec2 position)
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture("textures/example_content.jpg"),
    _transform(glm::vec3(position.x, position.y, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(static_cast<float>((float)screenWidth * size.x), static_cast<float>((float)screenHeight * size.y), 1.0f)),
    _size(size),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight)
{

    CalculateSize();
}

void ContentTile::CalculateSize() {
    _transform.scale.x = static_cast<float>(_size.x * _screenWidth);
    _transform.scale.y = static_cast<float>(_size.y * _screenHeight);
}

void ContentTile::Draw(glm::mat4 viewProjection) {
    _indexBuffer.Bind();
    _vertexBuffer.Bind();
    _texture.Bind();
    PositionUV::SetVertexAttribPointers();
    _shader->Bind();
    auto model = _transform.GetModelMatrix();
    _shader->SetShaderUniform("MVP", viewProjection * _transform.GetModelMatrix());

    glDrawElements(GL_TRIANGLES, _indexBuffer.GetNumFaces() * 3, GL_UNSIGNED_INT, 0);
}
