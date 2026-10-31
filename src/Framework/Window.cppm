module;

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <string>
#include <stdexcept>
#include <format>

export module Birdle.Framework:Window;

export namespace bdl
{
    class Window final
    {
        friend class Renderer;
        SDL_Window* Handle;

    public:
        Window(const std::string& title, const glm::ivec2& size)
        {
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
                throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

            Handle = SDL_CreateWindow(title.c_str(), size.x, size.y, SDL_WINDOW_OPENGL);
            if (!Handle)
                throw std::runtime_error(std::format("Failed to create window: {}", SDL_GetError()));

            // the game basically always wants text input
            // so just disable the on-screen keyboard and leave text input enabled
            SDL_SetHint(SDL_HINT_ENABLE_SCREEN_KEYBOARD, "0");
            SDL_StartTextInput(Handle);
        }

        ~Window()
        {
            SDL_StopTextInput(Handle);
            SDL_DestroyWindow(Handle);
            SDL_Quit();
        }
    };
}
