module;

#include <memory>

export module Birdle;
import Birdle.Framework;

export namespace bdl
{
    class BirdleGame : public Framework
    {
        std::unique_ptr<Texture> _texture;

    public:
        BirdleGame() : Framework("birdle", { 800, 600 })
        {
            _texture = Renderer->CreateTexture("/home/aqua/Pictures/BAGELMIP.png");
        }

        void Draw() override
        {
            Renderer->Clear(0.25f, 0.5f, 1.0f, 1.0f);
            Renderer->DrawSprite({
                .Texture = *_texture,
                .TopLeft = { 0, 0 },
                .TopRight = { 256, 0 },
                .BottomLeft = { 0, 256 },
                .BottomRight = { 256, 256 },
                .Tint = { 1.0f, 1.0f, 1.0f, 1.0f }
            });
        }
    };
}