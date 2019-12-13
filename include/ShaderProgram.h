#pragma once

#include <string>

namespace dss
{

/**
 *  A limited shader program abstraction over a Vertex Shader and Fragment Shader 
 * 
 *  NOTE: There is a lot of stuff that can be done with abstractions over shaders
 *  this class is very minimal, targeting only a vertex & fragment shader and some convenience
 *  methods & functionality
 */
class ShaderProgram {
private:
    unsigned int _programHandle;

    bool CompileAndLink(const char *vertexSource, const char *fragmentSource);
public:
    // TODO: Keep this deleted?
    ShaderProgram() = delete;

    ShaderProgram(const std::string &vertexSource, const std::string &fragmentSource);
};

}