#include "Background.h"
#include "QuadMesh.h"
#include "VertexFormat.h"

using namespace dss;

Background::Background(std::shared_ptr<ShaderProgram> shader) 
    :   TexturedDrawable(&QuadMesh(), "textures/background.jpg", shader)
{
    _transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Background::Draw(const glm::mat4 &view, const glm::mat4 &projection) {
    TexturedDrawable::DoDraw(_transform, view, projection);
}

void Background::SetSize(uint32_t width, uint32_t height) {
    _transform.scale = glm::vec3(static_cast<float>(width), static_cast<float>(height), 1.0f);
}