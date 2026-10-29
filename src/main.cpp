#include "Framework/Window.h"

int main(int argc, char* argv[])
{
    auto window = new bdl::Window("Test", 800, 600);

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
    }

    delete window;
}
