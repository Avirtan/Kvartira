
#include "Shader.h"
#include <SDL.h>
#include <fstream>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>
#include <sstream>

namespace Core {
Shader::Shader() : mShaderProgram(0), mVertexShader(0), mFragShader(0) {}

Shader::~Shader() { Unload(); }

bool Shader::Load(const std::string &vertName, const std::string &fragName) {
    if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) || !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader)) {
        return false;
    }

    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, mVertexShader);
    glAttachShader(mShaderProgram, mFragShader);
    glLinkProgram(mShaderProgram);

    // Verify that the program linked successfully
    if (!IsValidProgram()) {
        return false;
    }

    return true;
}

void Shader::Unload() {
    glDeleteProgram(mShaderProgram);
    glDeleteShader(mVertexShader);
    glDeleteShader(mFragShader);
}

void Shader::SetActive() {
    // Set this program as the active one
    glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char *name, const glm::mat4 &matrix) {
    GLuint loc = glGetUniformLocation(mShaderProgram, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

// void Shader::SetVectorUniform(const char* name, const Vector3& vector)
// {
// 	GLuint loc = glGetUniformLocation(mShaderProgram, name);
// 	// Send the vector data
// 	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
// }

void Shader::SetFloatUniform(const char *name, float value) {
    GLuint loc = glGetUniformLocation(mShaderProgram, name);
    glUniform1f(loc, value);
}

bool Shader::CompileShader(const std::string &fileName, GLenum shaderType, GLuint &outShader) {
    // Open file
    std::ifstream shaderFile(fileName);
    if (shaderFile.is_open()) {
        // Read all the text into a string
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        std::string contents = sstream.str();
        const char *contentsChar = contents.c_str();

        // Create a shader of the specified type
        outShader = glCreateShader(shaderType);
        // Set the source characters and try to compile
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if (!IsCompiled(outShader)) {
            SDL_Log("Failed to compile shader %s", fileName.c_str());
            return false;
        }
    } else {
        SDL_Log("Shader file not found: %s", fileName.c_str());
        return false;
    }

    return true;
}

bool Shader::IsCompiled(GLuint shader) {
    GLint status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetShaderInfoLog(shader, 511, nullptr, buffer);
        SDL_Log("GLSL Compile Failed:\n%s", buffer);
        return false;
    }

    return true;
}

bool Shader::IsValidProgram() {

    GLint status;
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
        SDL_Log("GLSL Link Status:\n%s", buffer);
        return false;
    }

    return true;
}
} // namespace Core
