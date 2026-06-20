#pragma once

#include <SDL3/SDL.h>

namespace birdle
{
    class Game final
    {
        bool _alive{};
        SDL_Window* _window{};
        
    public:
        ~Game();
        
        void init();
        void update(float dt);
        void draw();
        
        void run();
    };
}
