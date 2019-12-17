#pragma once

#include <string>
#include <memory>

#include <glm/vec2.hpp>

#include "Transform.h"
#include "imgui.h"


namespace dss
{

/**
 *  Provides very simple text-rendering capabilities, wrapping the glText library 
 */
class Text {
public:
    enum class TextPositioning {
        ABOVE,
        BELOW
    };
private:
    std::string _textString;
    glm::vec2 _position;
    std::shared_ptr<Transform> _parentTransform;
    Transform *_attachedTransform;
    TextPositioning _textPositioning;

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