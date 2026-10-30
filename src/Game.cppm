module;

#include <glm/glm.hpp>

#include <memory>

export module Birdle;
import Birdle.Framework;

export namespace bdl
{
    class BirdleGame : public Framework
    {
        std::unique_ptr<Font> _font;

    public:
        BirdleGame() : Framework("birdle", {800, 600})
        {
            _font = std::make_unique<Font>(*Renderer, "/home/aqua/Code/C#/Glimpse/src/Glimpse.Assets/Fonts/Roboto-Regular.ttf");
        }

        void Draw() override
        {
            Renderer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

            constexpr int spacing = 5;
            constexpr glm::ivec2 size { 50, 50 };
            constexpr glm::ivec2 startPos { (800 / 2 - ((size.x + spacing) * 5) / 2), 50 };

            for (int y = 0; y < 6; y++)
            {
                for (int x = 0; x < 5; x++)
                {
                    glm::ivec2 pos = startPos + glm::ivec2(x * (size.x + spacing), y * (size.y + spacing));
                    Renderer->DrawFilledRect(pos, size, { 0.05f, 0.05f, 0.05f, 1.0f });
                    Renderer->DrawRect(pos, size, 1, { 0.5f, 0.5f, 0.5f, 1.0f });
                }
            }

            const std::string text = "Hello World!\nThis is some text.\nthis is some more text.\ndsfgdghjkjnfghnjk";
            //const std::string text = "Hello World!";
            auto textSize = _font->MeasureText(75, text);
            Renderer->DrawFilledRect({ 0, 0 }, textSize, Color::Gray());
            _font->Draw({ 0, 0 }, 75, text, Color::White());
        }
    };
}