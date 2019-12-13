#include "ShaderProgram.h"
#include <gl/glew.h>
#include <iostream>

using namespace dss;

void PrintShaderCompileErrorOutput(int shaderHandle);
void PrintShaderLinkErrorOutput(int programHandle);

ShaderProgram::ShaderProgram(const std::string &vertexSource, const std::string &fragmentSource) {
    _programHandle = glCreateProgram();
    CompileAndLink(vertexSource.c_str(), fragmentSource.c_str());
}

bool ShaderProgram::CompileAndLink(const char *vertexSource, const char *fragmentSource) {
    // Create the VertexShader
    auto vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderHandle, 1, &vertexSource, nullptr);
    glCompileShader(vertexShaderHandle);

    int success;
    glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);

    if (!success) {
        PrintShaderCompileErrorOutput(vertexShaderHandle);
        return false;
    }

    // Create the Fragment Shader
    auto fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderHandle, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShaderHandle);

    glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);

    if (!success) {
        PrintShaderCompileErrorOutput(fragmentShaderHandle);
        return false;
    }

    // Link the shaders
    glAttachShader(_programHandle, vertexShaderHandle);
    glAttachShader(_programHandle, fragmentShaderHandle);

    glLinkProgram(_programHandle);

    glGetProgramiv(_programHandle, GL_LINK_STATUS, &success);

    if (!success) {
        PrintShaderLinkErrorOutput(_programHandle);
        return false;
    }

    return true;
}

void PrintShaderCompileErrorOutput(int shaderHandle) {
    char buffer[1024];
    glGetShaderInfoLog(shaderHandle, 1024, nullptr, buffer);

    std::cerr << "FAILED TO COMPILE SHADER. INFO: " << std::endl;
    std::cerr << buffer << std::endl;
}

void PrintShaderLinkErrorOutput(int programHandle) {
    char buffer[1024];
    glGetProgramInfoLog(programHandle, 1024, nullptr, buffer);

    std::cerr << "FAILED TO LINK SHADER. INFO: " << std::endl;
    std::cerr << buffer << std::endl;
}

void ShaderProgram::Bind() {
    glUseProgram(_programHandle);
}