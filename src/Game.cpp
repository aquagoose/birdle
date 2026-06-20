#include "Game.h"

#include <cassert>
#include <stdexcept>
#include <format>

namespace birdle
{
    Game::~Game()
    {
        delete graphics;
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    void Game::run()
    {
        assert(_window == nullptr);
        
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
            throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));
        
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0); // no need for depth and stencil buffers
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
        
        _window = SDL_CreateWindow("birdle", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
        if (!_window)
            throw std::runtime_error(std::format("Failed to create window: {}", SDL_GetError()));
        
        graphics = new Graphics(_window);
        
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
            
            graphics->clear(1.0f, 0.5f, 0.25f, 1.0f);
            graphics->present(1);
        }
    }
}
