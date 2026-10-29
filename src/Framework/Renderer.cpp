#include "Renderer.h"

#include <stdexcept>
#include <format>

namespace bdl
{
    Renderer::Renderer(Window& window) : _window(window)
    {
        _context = SDL_GL_CreateContext(_window.Handle);
        if (!_context)
            throw std::runtime_error(std::format("Failed to create context: {}", SDL_GetError()));

        if (!SDL_GL_MakeCurrent(_window.Handle, _context))
            throw std::runtime_error(std::format("Failed to make current: {}", SDL_GetError()));

        gladLoadGL(SDL_GL_GetProcAddress);
    }

    Renderer::~Renderer()
    {
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
