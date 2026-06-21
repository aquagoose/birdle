#pragma once

#include <glad/gl.h>

#include <string>
#include <span>

namespace birdle
{
    enum class InputElementType
    {
        Float1,
        Float2,
        Float3,
        Float4
    };
    
    struct InputElement
    {
        std::string name;
        InputElementType type;
        size_t offset;
    };
    
    class BufferShaderSet final
    {
        GLuint _vao;
        GLuint _vbo;
        GLuint _ebo;
        
        GLuint _shaderProgram;
        
    public:
        BufferShaderSet(size_t numVertices, size_t numIndices, const std::string& vertexShader, const std::string& fragmentShader, bool dynamic = false);
        ~BufferShaderSet();
        
        void setup_shader_attribs(size_t stride, const std::span<InputElement>& inputLayout);
    };
}