module;

#include <memory>
#include <cmath>

export module Birdle;
import Birdle.Framework;

export namespace bdl
{
    class BirdleGame : public Framework
    {
        std::unique_ptr<Texture> _texture;
        std::unique_ptr<Texture> _texture2;
        float _value;

    public:
        BirdleGame() : Framework("birdle", { 800, 600 })
        {
            _texture = Renderer->CreateTexture("/home/aqua/Pictures/BAGELMIP.png");
            _texture2 = Renderer->CreateTexture("/home/aqua/Pictures/awesomeface.png");
            _value = 0;
        }

        void Loop(float dt) override
        {
            _value += dt;
            if (_value >= M_PI * 2)
                _value -= M_PI * 2;
        }

        void Draw() override
        {
            Renderer->Clear(0.25f, 0.5f, 1.0f, 1.0f);

            for (int i = 0; i < 10; i++)
                Renderer->DrawSprite(Sprite(*_texture, { sin(_value + i) * 100 + (i * 10), i * 50.0f }));

            for (int i = 0; i < 10; i++)
                Renderer->DrawSprite(Sprite(*_texture2, { sin(_value + i) * 100 + (500 - i * 10), i * 50.0f }));
        }
    };
}