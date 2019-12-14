#include "Background.h"
#include "QuadMesh.h"
#include "VertexFormat.h"

using namespace dss;

Background::Background(std::shared_ptr<Window> window, std::shared_ptr<ShaderProgram> shader) 
    :   _mesh(std::make_unique<QuadMesh>()),
        _window(window),
        _shader(shader),
        _indexBuffer(_mesh.get()),
        _vertexBuffer(_mesh.get()),
        _texture("textures/background.jpg") 
{
    _transform.scale = glm::vec3(static_cast<float>(window->Width()), static_cast<float>(window->Height()), 1.0f);
}

void Background::Draw(glm::mat4 viewProjection) {
    _indexBuffer.Bind();
    _vertexBuffer.Bind();
    _texture.Bind();
    PositionUV::SetVertexAttribPointers();
    _shader->Bind();
    auto model = _transform.GetModelMatrix();
    _shader->SetShaderUniform("MVP", viewProjection * _transform.GetModelMatrix());

    glDrawElements(GL_TRIANGLES, _indexBuffer.GetNumFaces() * 3, GL_UNSIGNED_INT, 0);
}