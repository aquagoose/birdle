#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <string>

namespace bdl
{
    class Window final
    {
        friend class Renderer;

        SDL_Window* Handle;

    public:
        Window(const std::string& title, const glm::ivec2& size);
        ~Window();
    };
}
