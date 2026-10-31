module;

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include <memory>
#include <vector>
#include <iostream>

export module Birdle;
import Birdle.Framework;

export namespace bdl
{
    class BirdleGame : public Framework
    {
        enum class CharacterStatus
        {
            None, // no status
            InWord, // the character is in the word, but not at this location
            Correct // the character is in the word and is in the correct location
        };

        struct Character
        {
            char Chr;
            CharacterStatus Status;
        };

        static constexpr int WordLength = 5;
        static constexpr int NumWords = 6;

        std::unique_ptr<Font> _font;
        std::vector<Character> _chars;

        std::string _word;

        int _currentRow{};
        int _currentColumn{};

        void InsertChar(int row, int column, char c)
        {
            _chars[row * WordLength + column] = { .Chr = c, .Status = CharacterStatus::None };
        }

    public:
        BirdleGame() : Framework("birdle", {800, 600})
        {
            _font = std::make_unique<Font>(*Renderer, "Content/Roboto-Medium.ttf");
            _chars.resize(WordLength * NumWords);

            _word = "PLANE";
        }

        void Loop(float dt) override
        {
            for (char c : InputQueue)
            {
                if (_currentColumn + 1 > WordLength)
                    break;

                InsertChar(_currentRow, _currentColumn++, std::toupper(c));
            }

            InputQueue.clear();

            if (IsKeyPressed(SDLK_BACKSPACE))
            {
                if (_currentColumn - 1 >= 0)
                    InsertChar(_currentRow, --_currentColumn, 0);
            }
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
                const Character& character = _chars[i];

                glm::ivec2 pos = startPos + glm::ivec2(x * (size.x + spacing), y * (size.y + spacing));
                Renderer->DrawFilledRect(pos, size, { 0.05f, 0.05f, 0.05f, 1.0f });
                Renderer->DrawRect(pos, size, 1, { 0.5f, 0.5f, 0.5f, 1.0f });

                // get the character as a string view. is this a hack? probably. but it works!
                std::string_view chr(&_chars[i].Chr, 1);
                glm::uvec2 textSize = _font->MeasureText(50, chr);
                glm::ivec2 textPos = pos + size / 2 - static_cast<glm::ivec2>(textSize / 2U);
                _font->Draw(textPos, 50, chr, Color::White());
            }
        }
    };
}