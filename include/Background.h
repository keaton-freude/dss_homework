#pragma once

#include <memory>

#include "Window.h"
#include "Transform.h"
#include "glm/mat4x4.hpp"
#include "TexturedDrawable.h"

namespace dss
{

/**
 *  Draws a background image 
 */
class Background : public TexturedDrawable<PositionUV> {
private:
    // For the Background, we'll be modifying the scale parameter to ensure our
    // quad will cover the entire screen.
    Transform _transform;

    // Updates the scale in our transform to stretch the Mesh over the full window
    void UpdateTransform();
public:
    Background() = delete;
    // @window is the window we are painting a background over
    Background(std::shared_ptr<ShaderProgram> shader);

    virtual void Draw(const glm::mat4 &view, const glm::mat4 &projection) override;

    // This needs to be called when the size of the window content area changes
    void SetSize(uint32_t width, uint32_t height);
};

}