#pragma once

#include "Framework/Graphics.h"
#include "Framework/SpriteRenderer.h"

#include <SDL3/SDL.h>

namespace birdle
{
    class Game final
    {
        bool _alive{};
        SDL_Window* _window{};
        
    public:
        Graphics* graphics{}; // todo probably shouldn't be using raw pointers here
        SpriteRenderer* renderer{};
        
        ~Game();
        
        void init();
        void update(float dt);
        void draw();
        
        void run();
    };
}
