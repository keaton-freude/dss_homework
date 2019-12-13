#pragma once

#include <string_view>
#include <fstream>
#include <filesystem>

/**
 *  Small helper utilities 
 */

namespace dss
{

std::string FileReadAllText(std::filesystem::path path) {
    // Ensure file exists so we can give a nice error if we can't find it
    if (!std::filesystem::exists(path)) {
        std::cerr << "File not found: " << std::filesystem::absolute(path).string() << std::endl;
        throw std::runtime_error("File not found");
    }

    try {
        std::ifstream fs(path);
        return std::string(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
    } catch ([[maybe_unused]] const std::ifstream::failure& exception) {
        // TODO: Check that this exception works..
        // TODO: Revisit the maybe_unused piece!
        std::cerr << "Failed to open file, though it exists: " << path.filename().string() << std::endl;
        // TODO: re-throw?
        throw std::runtime_error("Failed to open file.");
    }
}

std::string GetResourcesRoot() {
    // Returns the path to the resources folder by making some guesses
    // NOTE: In a more robust application we'd have multiple ways of trying to find our installed
    // resources. Maybe environment variable.. registry setting, etc
    // In this case we will just look in the same directory as the currently running executable
    std::cout << "Current path: " << std::filesystem::current_path().string() << std::endl;
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
std::filesystem::path GetPathToResource(const std::string& resourceName) {
    return std::filesystem::path(GetResourcesRoot()) / std::filesystem::path(resourceName);
}

}