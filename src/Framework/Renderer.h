#pragma once

#include "Window.h"

#include <glad/gl.h>

namespace bdl
{
    class Renderer final
    {
        Window& _window;
        SDL_GLContext _context;

        // sprite renderer buffers
        GLuint _vao;
        GLuint _vbo;
        GLuint _ebo;
        GLuint _program;

    public:
        explicit Renderer(Window& window);
        ~Renderer();

        void Clear(float r, float g, float b, float a);

        void Present(uint32_t swapInterval);
    };
}
