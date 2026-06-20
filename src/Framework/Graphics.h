#pragma once

#include <SDL3/SDL.h>

namespace birdle
{
    class Graphics final
    {
        SDL_Window* _window;
        SDL_GLContext _context;
        
    public:
        explicit Graphics(SDL_Window* window);
        ~Graphics();
        
        void clear(float r, float g, float b, float a);
        
        void present(int interval);
    };
}
