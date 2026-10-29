#include "Window.h"

#include <stdexcept>
#include <format>

namespace bdl
{
    Window::Window(const std::string& title, int32_t width, int32_t height)
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
            throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));

        _window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
        if (!_window)
            throw std::runtime_error(std::format("Failed to create window: {}", SDL_GetError()));
    }

    Window::~Window()
    {
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }
}
