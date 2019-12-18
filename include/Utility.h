#pragma once

#include <string_view>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <sstream>

/**
 *  Small helper utilities 
 */

namespace dss
{

static inline std::string FileReadAllText(std::filesystem::path path) {
    // Ensure file exists so we can give a nice error if we can't find it
    if (!std::filesystem::exists(path)) {
        std::cerr << "File not found: " << std::filesystem::absolute(path).string() << std::endl;
        throw std::runtime_error("File not found");
    }

    try {
        std::ifstream fs(path);
        return std::string(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
    } catch ([[maybe_unused]] const std::ifstream::failure& exception) {
        std::cerr << "Failed to open file, though it exists: " << path.filename().string() << std::endl;
        std::stringstream exMessage;
        exMessage << "Failed to open file: " << path << "\nMessage: " << exception.what();
        throw std::runtime_error(exMessage.str().c_str());
    }
}

static inline std::string GetResourcesRoot() {
    // Returns the path to the resources folder by making some guesses
    // NOTE: In a more robust application we'd have multiple ways of trying to find our installed
    // resources. Maybe environment variable.. registry setting, etc
    // In this case we will just look in the same directory as the currently running executable
    const static std::string RESOURCES_ROOT = (std::filesystem::current_path() / "resources").string();
    const static bool resourceRootValid = std::filesystem::exists(RESOURCES_ROOT);
    if (!resourceRootValid) {
        throw std::runtime_error(std::string("Failed to find resources folder at: ") + RESOURCES_ROOT);
    }
    return RESOURCES_ROOT;
}

/**
 *  Convenience function for returning a fully-qualified path to a resource
 *  A resource is a file, available in the 'resources' folder and is namespaced
 *  Example resourceName: "shaders/basic.vert" or "textures/background.png" 
 */
static inline std::filesystem::path GetPathToResource(const std::string& resourceName) {
    return std::filesystem::path(GetResourcesRoot()) / std::filesystem::path(resourceName);
}

}