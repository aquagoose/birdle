module;

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL3/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <stdexcept>
#include <format>
#include <memory>
#include <cassert>
#include <optional>
#include <functional>

export module Birdle.Framework:Renderer;
import :Window;
import :Texture;
import :Types;

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
    out_Color = texture(uTexture, frag_TexCoord) * frag_Tint;
}
)";

    static constexpr uint32_t MaxSprites = 4096; // maximum sprites per batch
    static constexpr uint32_t NumVertices = 4; // number of vertices per sprite
    static constexpr uint32_t NumIndices = 6; // number of indices per sprite

    struct Vertex
    {
        glm::vec2 Position;
        glm::vec2 TexCoord;
        Color Tint;
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

    export struct Sprite
    {
        bdl::Texture& Texture;
        glm::vec2 TopLeft;
        glm::vec2 TopRight;
        glm::vec2 BottomLeft;
        glm::vec2 BottomRight;
        Color Tint;

        Sprite(bdl::Texture& texture, const glm::vec2& topLeft, const glm::vec2& topRight, const glm::vec2& bottomLeft, const glm::vec2& bottomRight, const Color& tint) :
            Texture(texture),
            TopLeft(topLeft),
            TopRight(topRight),
            BottomLeft(bottomLeft),
            BottomRight(bottomRight),
            Tint(tint) {}

        Sprite(bdl::Texture& texture, const glm::vec2& position, const glm::vec2& size, const Color& tint = Color::White())
            : Texture(texture), Tint(tint)
        {
            TopLeft = position;
            TopRight = { position.x + size.x, position.y };
            BottomLeft = { position.x, position.y + size.y };
            BottomRight = { position.x + size.x, position.y + size.y };
        }

        inline Sprite(bdl::Texture& texture, const glm::vec2& position, const Color& tint = Color::White())
            : Sprite(texture, position, texture.Size(), tint) {}
    };

    export class Renderer final
    {
        Window& _window;
        SDL_GLContext _context;

        // sprite renderer buffers
        GLuint _vao;
        GLuint _vbo;
        GLuint _ebo;
        GLuint _program;
        std::vector<Sprite> _sprites;
        std::array<Vertex, MaxSprites * NumVertices> _vertices{};
        std::array<uint16_t, MaxSprites * NumIndices> _indices{};

        std::unique_ptr<Texture> _whiteTexture;

    public:
        explicit Renderer(Window& window) : _window(window)
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
            glBufferData(GL_ARRAY_BUFFER, MaxSprites * NumVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

            glGenBuffers(1, &_ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxSprites * NumIndices * sizeof(uint16_t), nullptr, GL_DYNAMIC_DRAW);

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

            GLuint aPosition = glGetAttribLocation(_program, "aPosition");
            glEnableVertexAttribArray(aPosition);
            glVertexAttribPointer(aPosition, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));

            GLuint aTexCoord = glGetAttribLocation(_program, "aTexCoord");
            glEnableVertexAttribArray(aTexCoord);
            glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoord)));

            GLuint aTint = glGetAttribLocation(_program, "aTint");
            glEnableVertexAttribArray(aTint);
            glVertexAttribPointer(aTint, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Tint)));

            uint8_t white[] = { 255, 255, 255, 255 };
            _whiteTexture = CreateTexture(white, { 1, 1 }, PixelFormat::RGBA8);
        }

        ~Renderer()
        {
            glDeleteProgram(_program);
            glDeleteBuffers(1, &_ebo);
            glDeleteBuffers(1, &_vbo);
            glDeleteVertexArrays(1, &_vao);

            SDL_GL_DestroyContext(_context);
        }

        std::unique_ptr<Texture> CreateTexture(void* data, const glm::ivec2& size, PixelFormat format)
        {
            return std::unique_ptr<Texture>(new Texture(data, size, format));
        }

        std::unique_ptr<Texture> CreateTexture(const std::string& path)
        {
            int w, h;
            void* data = stbi_load(path.c_str(), &w, &h, nullptr, 4);
            if (!data)
                throw std::runtime_error(std::format("Failed to load texture: {}", stbi_failure_reason()));

            auto texture = std::unique_ptr<Texture>(new Texture(data, { w, h }, PixelFormat::RGBA8));
            stbi_image_free(data);

            return texture;
        }

        void Clear(const Color& color)
        {
            glClearColor(color.R, color.G, color.B, color.A);
            glClear(GL_COLOR_BUFFER_BIT);
        }


        inline void DrawSprite(const Sprite& sprite)
        {
            _sprites.push_back(sprite);
        }

        inline void DrawFilledRect(const glm::vec2& pos, const glm::vec2& size, const Color& color)
        {
            _sprites.emplace_back(*_whiteTexture, pos, size, color);
        }

        void DrawRect(const glm::vec2& pos, const glm::vec2& size, int32_t borderSize, const Color& color)
        {
            // the lines are drawn in this order:
            // ----1----
            // |       |
            // 2       4
            // |       |
            // ----3----
            _sprites.emplace_back(*_whiteTexture, pos, glm::vec2(size.x, borderSize), color);
            _sprites.emplace_back(*_whiteTexture, pos, glm::vec2(borderSize, size.y), color);
            _sprites.emplace_back(*_whiteTexture, glm::vec2(pos.x, pos.y + size.y - borderSize), glm::vec2(size.x, borderSize), color);
            _sprites.emplace_back(*_whiteTexture, glm::vec2(pos.x + size.x - borderSize, pos.y), glm::vec2(borderSize, size.y), color);
        }

        void Present(uint32_t swapInterval)
        {
            Render();

            SDL_GL_SetSwapInterval(static_cast<int>(swapInterval));
            SDL_GL_SwapWindow(_window.Handle);
        }

    private:
        void Render()
        {
            // don't even bother
            if (_sprites.empty())
                return;

            std::optional<std::reference_wrapper<Texture>> currentTexture{};
            uint32_t numSprites = 0;
            for (const auto& sprite : _sprites)
            {
                if (numSprites > 0 && &sprite.Texture != &currentTexture->get())
                {
                    Flush(*currentTexture, numSprites);
                    numSprites = 0;
                }

                currentTexture = sprite.Texture;

                uint32_t vOffset = numSprites * NumVertices;
                uint32_t iOffset = numSprites * NumIndices;

                _vertices[vOffset + 0] = { .Position = sprite.TopLeft, .TexCoord = { 0, 0 }, .Tint = sprite.Tint };
                _vertices[vOffset + 1] = { .Position = sprite.TopRight, .TexCoord = { 1, 0 }, .Tint = sprite.Tint };
                _vertices[vOffset + 2] = { .Position = sprite.BottomRight, .TexCoord = { 1, 1 }, .Tint = sprite.Tint };
                _vertices[vOffset + 3] = { .Position = sprite.BottomLeft, .TexCoord = { 0, 1 }, .Tint = sprite.Tint };

                _indices[iOffset + 0] = static_cast<uint16_t>(0 + vOffset);
                _indices[iOffset + 1] = static_cast<uint16_t>(1 + vOffset);
                _indices[iOffset + 2] = static_cast<uint16_t>(3 + vOffset);
                _indices[iOffset + 3] = static_cast<uint16_t>(1 + vOffset);
                _indices[iOffset + 4] = static_cast<uint16_t>(2 + vOffset);
                _indices[iOffset + 5] = static_cast<uint16_t>(3 + vOffset);

                numSprites++;
            }

            assert(currentTexture.has_value());
            Flush(*currentTexture, numSprites);

            _sprites.clear();
        }

        void Flush(Texture& texture, uint32_t numSprites) const
        {
            glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

            glBindVertexArray(_vao);
            glUseProgram(_program);

            glBufferSubData(GL_ARRAY_BUFFER, 0, numSprites * NumVertices * sizeof(Vertex), _vertices.data());
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numSprites * NumIndices * sizeof(uint16_t), _indices.data());

            glUniformMatrix4fv(glGetUniformLocation(_program, "uProjection"), 1, false, glm::value_ptr(projection));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.Handle());

            glDrawElements(GL_TRIANGLES, static_cast<GLint>(numSprites * NumIndices), GL_UNSIGNED_SHORT, nullptr);
        }
    };
}
