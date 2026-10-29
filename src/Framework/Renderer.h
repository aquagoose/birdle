#pragma once

#include "Window.h"

#include <glad/gl.h>

#include <vector>

namespace bdl
{
    struct Sprite
    {
        glm::ivec2 TopLeft;
        glm::ivec2 TopRight;
        glm::ivec2 BottomLeft;
        glm::ivec2 BottomRight;
        glm::ivec4 Tint;
    };

    class Renderer final
    {
        Window& _window;
        SDL_GLContext _context;

        // sprite renderer buffers
        GLuint _vao;
        GLuint _vbo;
        GLuint _ebo;
        GLuint _program;
        std::vector<Sprite> _sprites;

    public:
        explicit Renderer(Window& window);
        ~Renderer();

        void Clear(float r, float g, float b, float a);

        void DrawSprite(const Sprite& sprite);

        void Present(uint32_t swapInterval);

    private:
        void Render();
        void Flush(uint32_t numSprites) const;
    };
}
