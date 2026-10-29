#include "Renderer.h"

#include <stdexcept>
#include <format>

namespace bdl
{
    static const char* VertexShader = R"(
#version 330 core

in vec2 aPosition;
in vec2 aTexCoord;
in vec4 aTint;

out vec2 frag_TexCoord;
out vec4 frag_Tint;

uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * vec4(aPosition, 0.0, 1.0);
    frag_TexCoord = aTexCoord;
    frag_Tint = aTint;
}
)";

    static const char* FragmentShader = R"(
#version 330 core

in vec2 frag_TexCoord;
in vec4 frag_Tint;

out vec4 out_Color;

uniform sampler2D uTexture;

void main()
{
    out_Color = vec4(1.0);
}
)";

    static constexpr uint32_t MaxSprites = 4096; // maximum sprites per batch
    static constexpr uint32_t NumVertices = 4; // number of vertices per sprite
    static constexpr uint32_t NumIndices = 6; // number of indices per sprite

    struct Sprite
    {
        glm::vec2 Position;
        glm::vec2 TexCoord;
        glm::vec4 Tint;
    };

    GLuint CreateShader(GLenum type, const char* source)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            char infoLog[1000];
            glGetShaderInfoLog(shader, 1000, nullptr, infoLog);
            throw std::runtime_error(std::format("Failed to compile shader: {}", infoLog));
        }

        return shader;
    }

    Renderer::Renderer(Window& window) : _window(window)
    {
        _context = SDL_GL_CreateContext(_window.Handle);
        if (!_context)
            throw std::runtime_error(std::format("Failed to create context: {}", SDL_GetError()));

        if (!SDL_GL_MakeCurrent(_window.Handle, _context))
            throw std::runtime_error(std::format("Failed to make current: {}", SDL_GetError()));

        gladLoadGL(SDL_GL_GetProcAddress);

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, MaxSprites * NumVertices * sizeof(Sprite), nullptr, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxSprites * NumIndices * sizeof(int16_t), nullptr, GL_DYNAMIC_DRAW);

        _program = glCreateProgram();
        GLuint vShader = CreateShader(GL_VERTEX_SHADER, VertexShader);
        GLuint fShader = CreateShader(GL_FRAGMENT_SHADER, FragmentShader);

        glAttachShader(_program, vShader);
        glAttachShader(_program, fShader);
        glLinkProgram(_program);

        GLint status;
        glGetProgramiv(_program, GL_LINK_STATUS, &status);
        if (!status)
        {
            char infoLog[1000];
            glGetProgramInfoLog(_program, 1000, nullptr, infoLog);
            throw std::runtime_error(std::format("Failed to link program: {}", infoLog));
        }

        glDetachShader(_program, fShader);
        glDetachShader(_program, vShader);
        glDeleteShader(fShader);
        glDeleteShader(vShader);
    }

    Renderer::~Renderer()
    {
        glDeleteProgram(_program);
        glDeleteBuffers(1, &_ebo);
        glDeleteBuffers(1, &_vbo);
        glDeleteVertexArrays(1, &_vao);

        SDL_GL_DestroyContext(_context);
    }

    void Renderer::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::Present(uint32_t swapInterval)
    {
        SDL_GL_SetSwapInterval(static_cast<int>(swapInterval));
        SDL_GL_SwapWindow(_window.Handle);
    }
}
