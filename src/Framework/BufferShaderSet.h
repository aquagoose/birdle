#pragma once

#include <glad/gl.h>

#include <string>

namespace birdle
{
    class BufferShaderSet final
    {
        GLuint _vao;
        GLuint _vbo;
        GLuint _ebo;
        
        GLuint _shaderProgram;
        
    public:
        BufferShaderSet(size_t numVertices, size_t numIndices, const std::string& vertexShader, const std::string& fragmentShader, bool dynamic = false);
        ~BufferShaderSet();
    };
}