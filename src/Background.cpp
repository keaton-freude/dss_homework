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
    
    _transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Background::Draw(glm::mat4 view, glm::mat4 projection) {
    _indexBuffer.Bind();
    _vertexBuffer.Bind();
    _texture.Bind();
    PositionUV::SetVertexAttribPointers();
    _shader->Bind();
    auto w = _window->Width();
    auto h = _window->Height();
    auto model = _transform.GetModelMatrix();

    _shader->SetShaderUniform("Model", model);
    _shader->SetShaderUniform("View", view);
    _shader->SetShaderUniform("Projection", projection);

    glDrawElements(GL_TRIANGLES, _indexBuffer.GetNumFaces() * 3, GL_UNSIGNED_INT, 0);
}

void Background::SetSize(uint32_t width, uint32_t height) {
    _transform.scale = glm::vec3(static_cast<float>(width), static_cast<float>(height), 1.0f);
}