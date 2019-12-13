#pragma once

#include <string>

namespace dss
{

/**
 *  Scope-limited texture class: Only support basic 2D texture 
 */
class Texture {
private:
    unsigned int _textureHandle;
public:
    Texture() = delete;
    Texture(const std::string &textureName);

    void Bind();
};

}