#include "Game.h"

#include <cassert>
#include <stdexcept>
#include <format>

namespace birdle
{
    Game::~Game()
    {
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    void Game::run()
    {
        assert(_window == nullptr);
        
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
            throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));
        
        _window = SDL_CreateWindow("birdle", 800, 600, SDL_WINDOW_RESIZABLE);
        if (!_window)
            throw std::runtime_error(std::format("Failed to create window: {}", SDL_GetError()));
        
        _alive = true;
        while (_alive)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_EVENT_QUIT:
                    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                        _alive = false;
                        break;
                }
            }
        }
    }
}
