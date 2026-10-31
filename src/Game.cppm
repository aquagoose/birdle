module;

#include <glm/glm.hpp>

#include <memory>
#include <vector>

export module Birdle;
import Birdle.Framework;

export namespace bdl
{
    class BirdleGame : public Framework
    {
        static constexpr int WordLength = 5;
        static constexpr int NumWords = 6;

        std::unique_ptr<Font> _font;
        std::vector<char> _chars;

        void InsertChar(int row, int column, char c)
        {
            _chars[row * WordLength + column] = c;
        }

    public:
        BirdleGame() : Framework("birdle", {800, 600})
        {
            _font = std::make_unique<Font>(*Renderer, "Content/Roboto-Medium.ttf");

            _chars.resize(WordLength * NumWords);
            InsertChar(0, 0, 'C');
            InsertChar(0, 1, 'H');
            InsertChar(0, 2, 'O');
            InsertChar(0, 3, 'P');
            InsertChar(0, 4, 'S');

            InsertChar(1, 0, 'P');
            InsertChar(1, 1, 'L');
            InsertChar(1, 2, 'A');
            InsertChar(1, 3, 'N');
            InsertChar(1, 4, 'E');

            InsertChar(2, 0, 'A');
            InsertChar(2, 1, 'N');
            InsertChar(2, 2, 'G');
        }

        void Draw() override
        {
            Renderer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

            constexpr int spacing = 5;
            constexpr glm::ivec2 size { 50, 50 };
            constexpr glm::ivec2 startPos { (800 / 2 - ((size.x + spacing) * 5) / 2), 50 };

            for (int i = 0; i < WordLength * NumWords; i++)
            {
                int x = i % WordLength;
                int y = i / WordLength;
                glm::ivec2 pos = startPos + glm::ivec2(x * (size.x + spacing), y * (size.y + spacing));
                Renderer->DrawFilledRect(pos, size, { 0.05f, 0.05f, 0.05f, 1.0f });
                Renderer->DrawRect(pos, size, 1, { 0.5f, 0.5f, 0.5f, 1.0f });

                std::string_view chr(&_chars[i], 1);
                glm::uvec2 textSize = _font->MeasureText(50, chr);
                glm::ivec2 textPos = pos + size / 2 - static_cast<glm::ivec2>(textSize / 2U);
                _font->Draw(textPos, 50, chr, Color::White());
            }
        }
    };
}