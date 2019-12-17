#pragma once
#include <vector>
#include <string>

namespace dss
{

struct ContentTileData {
    std::string title;
    std::string blurb;
    std::vector<unsigned char> textureData;

    // Move the 'textureData' (which is the raw texture data) into this class, so we don't incur a copy
    ContentTileData(const std::string &title, const std::string &blurb, const std::vector<unsigned char>&& textureData) 
        :title(title), blurb(blurb), textureData(std::move(textureData)) {

    }
};

}