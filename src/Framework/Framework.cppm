module;

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <deque>
#include <unordered_set>

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

        std::unordered_set<SDL_Keycode> _keysPressed;
        std::deque<char> InputQueue;

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
                _keysPressed.clear();

                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                        case SDL_EVENT_QUIT:
                            _running = false;
                            break;

                        case SDL_EVENT_KEY_DOWN:
                        {
                            if (event.key.repeat)
                                break;
                            _keysPressed.emplace(event.key.key);
                            break;
                        }
                        case SDL_EVENT_KEY_UP:
                        {
                            _keysPressed.erase(event.key.key);
                            break;
                        }

                        case SDL_EVENT_TEXT_INPUT:
                        {
                            size_t length = SDL_strlen(event.text.text);
                            for (size_t i = 0; i < length; i++)
                                InputQueue.push_back(event.text.text[i]);
                        }
                    }
                }

                Loop(1.0f / 60.0f);
                Draw();

                Renderer->Present(1);
            }
        }

        inline bool IsKeyPressed(SDL_Keycode key) const { return _keysPressed.contains(key); }
    };
}