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
    CreateTexture(data, x, y); 

    stbi_image_free(data);
}

Texture::Texture(std::vector<unsigned char> &&textureData) {
    int x, y, n;
    unsigned char *data = stbi_load_from_memory(&textureData.front(), static_cast<int>(textureData.size()), &x, &y, &n, 0);
    
    if (!data) {
        std::cerr << "Failed to load image!" << std::endl;
    }
    CreateTexture(data, x, y); 

    stbi_image_free(data);
}

void Texture::CreateTexture(unsigned char *data, int width, int height) {
    glGenTextures(1, &_textureHandle);
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
}