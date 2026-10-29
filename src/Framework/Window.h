#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace bdl
{
    class Window final
    {
        SDL_Window* _window;

    public:
        Window(const std::string& title, int32_t width, int32_t height);
        ~Window();
    };
}
