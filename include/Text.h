#pragma once

#include <string>
#include <memory>

#include <glm/vec2.hpp>

#include "Transform.h"


namespace dss
{

/**
 *  Provides very simple text-rendering capabilities, wrapping the glText library 
 */
class Text {
private:
    std::string _textString;
    glm::vec2 _position;
    std::shared_ptr<Transform> _parentTransform;

public:
    Text() = delete;
    Text(const std::string &string, glm::vec2 position, std::shared_ptr<Transform> parentTransform);

    void Draw();

    void SetPosition(glm::vec2 position) {
        _position = position;
    }
};

}