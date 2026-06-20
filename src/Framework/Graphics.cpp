#include "Graphics.h"

#include <glad/gl.h>

#include <stdexcept>
#include <format>

namespace birdle
{
    Graphics::Graphics(SDL_Window* window) : _window(window)
    {
        _context = SDL_GL_CreateContext(_window);
        if (!_context)
            throw std::runtime_error(std::format("Failed to create GL context: {}", SDL_GetError()));
        
        gladLoadGL(SDL_GL_GetProcAddress);
    }

    Graphics::~Graphics()
    {
        SDL_GL_DestroyContext(_context);
    }

    void Graphics::clear(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Graphics::present(const int interval)
    {
        // todo perform error checking
        SDL_GL_SetSwapInterval(interval);
        SDL_GL_SwapWindow(_window);
    }
}
