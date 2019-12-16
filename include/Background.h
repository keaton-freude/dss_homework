#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Window.h"
#include "Transform.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "TexturedDrawable.h"
#include <memory>

namespace dss
{

/**
 *  Draws a background image 
 */
class Background : public TexturedDrawable<PositionUV> {
private:
    // TODO FIX
    std::shared_ptr<Window> _window;

    // For the Background, we'll be modifying the scale parameter to ensure our
    // quad will cover the entire screen.
    Transform _transform;

    // Updates the scale in our transform to stretch the Mesh over the full window
    void UpdateTransform();
public:
    Background() = delete;
    // @window is the window we are painting a background over
    Background(std::shared_ptr<Window> window, std::shared_ptr<ShaderProgram> shader);

    virtual void Draw(const glm::mat4 &view, const glm::mat4 &projection) override;

    void SetSize(uint32_t width, uint32_t height);
};

}