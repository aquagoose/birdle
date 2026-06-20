#include "BufferShaderSet.h"

#include <cassert>
#include <stdexcept>
#include <format>

GLuint create_shader(const GLenum shaderType, const std::string& source)
{
    auto shader = glCreateShader(shaderType);
    auto sourceStr = source.c_str();
    GLint length = source.length();
    glShaderSource(shader, 1, &sourceStr, &length);
    glCompileShader(shader);
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    if (status == GL_TRUE)
        return shader; // no error
    
    // simpler to just stackalloc a char buffer
    // meh we don't need more than 1000 chars right
    char shaderLog[1000];
    glGetShaderInfoLog(shader, 1000, nullptr, shaderLog);
    throw std::runtime_error(std::format("Failed to compile shader: {}", shaderLog));
}

namespace birdle
{
    BufferShaderSet::BufferShaderSet(const size_t numVertices, const size_t numIndices, const std::string& vertexShader, const std::string& fragmentShader, const bool dynamic)
    {
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        
        // must have at least 1 vertex
        assert(numVertices > 0);
        
        const auto bufferUsage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
        
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, numVertices, nullptr, bufferUsage); // create empty buffer
        
        // index buffer is not required
        if (numIndices > 0)
        {
            glGenBuffers(1, &_ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ARRAY_BUFFER, numIndices, nullptr, bufferUsage);
        }
        else
            _ebo = 0; // ensure the default value cause c++
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        _shaderProgram = glCreateProgram();
        auto vShader = create_shader(GL_VERTEX_SHADER, vertexShader);
        auto fShader = create_shader(GL_FRAGMENT_SHADER, fragmentShader);
        
        glAttachShader(_shaderProgram, vShader);
        glAttachShader(_shaderProgram, fShader);
        glLinkProgram(_shaderProgram);
        
        // make sure it linked correctly
        GLint linkStatus;
        glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
            char shaderLog[1000];
            glGetProgramInfoLog(_shaderProgram, 1000, nullptr, shaderLog);
            throw std::runtime_error(std::format("Failed to link program: {}", shaderLog));
        }
        
        glDetachShader(_shaderProgram, vShader);
        glDetachShader(_shaderProgram, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }

    BufferShaderSet::~BufferShaderSet()
    {
        glBindVertexArray(0);
        glUseProgram(0);
        
        glDeleteProgram(_shaderProgram);
        if (_ebo != 0)
            glDeleteBuffers(1, &_ebo);
        glDeleteBuffers(1, &_vbo);
        glDeleteVertexArrays(1, &_vao);
    }
}
