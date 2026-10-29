#include "Framework/Renderer.h"
#include "Framework/Window.h"

int main(int argc, char* argv[])
{
    auto window = new bdl::Window("Test", { 800, 600 });
    auto renderer = new bdl::Renderer(*window);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }

        renderer->Clear(1.0f, 0.5f, 0.25f, 1.0f);
        renderer->Present(1);
    }

    delete renderer;
    delete window;
}
