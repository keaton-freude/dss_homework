#pragma once

#include <memory>
#include <string>

#include "glm/mat4x4.hpp"

#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "VertexFormat.h"

namespace dss
{

/**
 *  Represents the ability to draw a textured mesh
 * 
 *  @Type VertexLayoutTy is the underlying vertex type. By default
 *  for a textured drawable, we'll only consider Position & UV (the required
 *  vertex attributes for drawing a textured mesh). Can safely override with
 *  any type specified like the types in "VertexFormat.h"
 */
template <typename VertexLayoutTy = PositionUV>
class TexturedDrawable {
private:
    VertexBuffer _vertexBuffer;
    IndexBuffer _indexBuffer;
    Texture _texture;
    std::shared_ptr<ShaderProgram> _shader;

protected:
    // Perform the actual drawing. Derived classes should call this when they are ready to draw
    // and have all of the necessary parameters prepared
    void DoDraw(const Transform &transform, const glm::mat4 &view, const glm::mat4 &projection) {
        _indexBuffer.Bind();
        _vertexBuffer.Bind();
        _texture.Bind();
        VertexLayoutTy::SetVertexAttribPointers();
        _shader->Bind();

        _shader->SetShaderUniform("Model", transform.GetModelMatrix());
        _shader->SetShaderUniform("View", view);
        _shader->SetShaderUniform("Projection", projection);

        // Cast to int, because OpenGL requires an int
        glDrawElements(GL_TRIANGLES, static_cast<int>(_indexBuffer.GetNumFaces() * 3), GL_UNSIGNED_INT, 0);

    }

public:
    TexturedDrawable() = delete;

    TexturedDrawable(IMesh *mesh, std::vector<unsigned char> &&textureData, std::shared_ptr<ShaderProgram> shader) 
        :   _vertexBuffer(mesh),
            _indexBuffer(mesh),
            _texture(std::move(textureData)),
            _shader(shader)
    {

    }

    TexturedDrawable(IMesh *mesh, std::string texturePath, std::shared_ptr<ShaderProgram> shader) 
        :   _vertexBuffer(mesh),
            _indexBuffer(mesh),
            _texture(texturePath),
            _shader(shader)
    {
    }

    // User must specify an implementation for this method
    // At minimum they must provide a transform and call DoDraw
    // NOTE: Any way to more elegantly setup this behavior?
    virtual void Draw(const glm::mat4 &view, const glm::mat4 &projection) = 0; 
};

}