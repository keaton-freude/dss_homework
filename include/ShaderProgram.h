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
    // Enforce that instances of ShaderPrograms are always initialized and ready for use
    ShaderProgram() = delete;

    ShaderProgram(const std::string &vertexSource, const std::string &fragmentSource);

    void Bind();

    // Normally we'd have something like template partial specialization to map the
    // glm type to the specific OpenGL call. In this case though the only uniforms
    // we will be setting are Mat4s
    void SetShaderUniformMat4(const std::string &uniformName, glm::mat4x4 value) {
        auto loc = glGetUniformLocation(_programHandle, uniformName.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
    }
};

}