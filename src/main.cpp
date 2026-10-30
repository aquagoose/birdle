import Framework;

#include <SDL3/SDL.h>

int main(int argc, char* argv[])
{
    auto window = new bdl::Window("Test", { 800, 600 });
    auto renderer = new bdl::Renderer(*window);

    auto texture = renderer->CreateTexture("/home/aqua/Pictures/BAGELMIP.png");

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
        renderer->DrawSprite({
            .Texture = *texture,
            .TopLeft = {0, 0},
            .TopRight = {100, 0},
            .BottomLeft = {0, 100},
            .BottomRight = {100, 100},
            .Tint = {1.0f, 1.0f, 1.0f, 1.0f}
        });
        renderer->Present(1);
    }

    texture.reset();
    delete renderer;
    delete window;
}
