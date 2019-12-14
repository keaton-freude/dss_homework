#include "ContentTile.h"
#include "QuadMesh.h"
#include "Utility.h"

using namespace dss;

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader)
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture("textures/example_content.jpg"),
    _transform()
{
    QuadMesh mesh;
    _indexBuffer = IndexBuffer(&mesh);
    _vertexBuffer = VertexBuffer(&mesh);
}

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader, glm::vec3 position, uint32_t width, uint32_t height) 
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture("textures/example_content.jpg"),
    _transform(position, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(static_cast<float>(width), static_cast<float>(height), 1.0f))
{
}

void ContentTile::CalculateSize() {
    _transform.scale.x = static_cast<float>(_width);
    _transform.scale.y = static_cast<float>(_height);
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
