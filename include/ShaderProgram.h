#pragma once

#include <string>
#include <gl/glew.h>
#include <glm/glm.hpp>

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

    void Bind();

    void SetShaderUniform(const std::string &uniformName, glm::mat4x4 value) {
        // TODO REPLACE ME, TESTING ONLY
        auto loc = glGetUniformLocation(_programHandle, uniformName.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
    }
};

}