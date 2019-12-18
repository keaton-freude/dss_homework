#pragma once

#include <string>
#include <memory>

#include <glm/vec2.hpp>

#include "Transform.h"
#include "imgui.h"

namespace dss
{

/**
 *  Provides very simple text rendering capability, using ImGUI to render the text
 *  This class requires that ImGUI is setup correctly for the windowing-system and
 *  graphics API in use. Render text to a full-screen, invisible ImGUI window
 */
class Text {
public:
    // Whether text should be positioned above or below the "attachedTransform"
    enum class TextPositioning {
        ABOVE,
        BELOW
    };
private:
    // Text to render
    std::string _textString;

    // Local positioning of the text
    glm::vec2 _position;

    // Inherit the parent transforms when positioning ourself
    std::shared_ptr<Transform> _parentTransform;

    // The transform to refer to when rendering above or below something
    Transform *_attachedTransform;

    // Whether we should render above or below the attachedTransform
    TextPositioning _textPositioning;

    // The font we will use to render text
    ImFont * _font;

public:
    Text() = delete;
    Text(const std::string &string, glm::vec2 position, std::shared_ptr<Transform> parentTransform, 
        Transform *attachedTransform, TextPositioning textPositioning, ImFont *font);

    void Draw();

    void SetPosition(glm::vec2 position) {
        _position = position;
    }
};

}