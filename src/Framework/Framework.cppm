module;

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include <memory>
#include <string>

export module Birdle.Framework;
export import :Window;
export import :Renderer;
export import :Texture;
export import :Types;
export import :Font;

export namespace bdl
{
    class Framework
    {
        bool _running{};

    public:
        std::unique_ptr<bdl::Window> Window{};
        std::unique_ptr<bdl::Renderer> Renderer{};

        Framework(const std::string& appName, const glm::ivec2& windowSize)
        {
            Window = std::make_unique<bdl::Window>(appName, windowSize);
            Renderer = std::make_unique<bdl::Renderer>(*Window);
        }

        virtual ~Framework() = default;

        virtual void Loop(float dt) {}
        virtual void Draw() {}

        void Run()
        {
            _running = true;
            while (_running)
            {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                        case SDL_EVENT_QUIT:
                            _running = false;
                            break;
                    }
                }

                Loop(1.0f / 60.0f);
                Draw();

                Renderer->Present(1);
            }
        }
    };
}