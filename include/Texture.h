#pragma once

#include <string>
#include <vector>

namespace dss
{

/**
 *  Scope-limited texture class: Only support basic 2D texture 
 */
class Texture {
private:
    unsigned int _textureHandle;
    void CreateTexture(unsigned char *data, int width, int height);
public:
    Texture() = delete;
    // Create from file
    Texture(const std::string &textureName);
    // Create from raw data
    Texture(std::vector<unsigned char> &&textureData);

    void Bind();
};

}