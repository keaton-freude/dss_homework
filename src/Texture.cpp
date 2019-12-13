#include "Texture.h"
#include "Utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <gl/glew.h>

using namespace dss;

Texture::Texture(const std::string &textureName) {
    // Ensure the resource exists and get its full path
    auto path = GetPathToResource(textureName).string();


    std::cout << "Loading Texture at path: " << path << std::endl;
    // Get the image data
    int x, y, n;
    unsigned char *data = stbi_load(path.c_str(), &x, &y, &n, 0);

    if (!data) {
        std::cerr << "Failed to load image!" << std::endl;
    }

    glGenTextures(1, &_textureHandle);
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
}