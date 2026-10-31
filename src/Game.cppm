module;

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

export module Birdle;
import :Utils;
import Birdle.Framework;

export namespace bdl
{
    class BirdleGame : public Framework
    {
        enum class CharacterStatus
        {
            None, // no status
            InWord, // the character is in the word, but not at this location
            Correct, // the character is in the word and is in the correct location
            Invalid // the character is not valid
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

        std::vector<std::string> _wordbank;
        std::string _word;

        int _currentRow{};
        int _currentColumn{};

        void InsertChar(int row, int column, char c)
        {
            _chars[row * WordLength + column] = { .Chr = c, .Status = CharacterStatus::None };
        }

        bool CheckWord()
        {
            assert(_word.length() == WordLength);
            assert(_currentColumn == WordLength);
            bool correct = true;

            for (int i = 0; i < WordLength; i++)
            {
                Character& character = _chars[_currentRow * WordLength + i];
                if (character.Chr == _word[i])
                    character.Status = CharacterStatus::Correct;
                else if (_word.find(character.Chr) != std::string::npos)
                {
                    correct = false;
                    character.Status = CharacterStatus::InWord;
                }
                else
                    correct = false;
            }

            return correct;
        }

    public:
        BirdleGame() : Framework("birdle", {800, 600})
        {
            _font = std::make_unique<Font>(*Renderer, "Content/Roboto-Medium.ttf");
            _chars.resize(WordLength * NumWords);

            _wordbank = Utils::LoadWordbank("Content/Banks/5.wb");

            // yes i am using srand no i am not doing anything about it
            srand(time(nullptr));
            int wordIndex = rand() % _wordbank.size();
            _word = _wordbank[wordIndex];
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

            if (IsKeyPressed(SDLK_RETURN))
            {
                if (_currentColumn < WordLength)
                {
                    for (int i = 0; i < WordLength; i++)
                    {
                        Character& character = _chars[_currentRow * WordLength + i];
                        character.Status = CharacterStatus::Invalid;
                    }
                }
                else
                {
                    if (CheckWord())
                    {
                        std::cout << "You got it right!" << std::endl;
                        Close();
                    }
                    else
                    {
                        _currentColumn = 0;
                        _currentRow++;

                        if (_currentRow >= NumWords)
                        {
                            std::cout << "You lost! The word was " << _word << std::endl;
                            Close();
                        }
                    }
                }
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

                Color bgColor = { 0.05f, 0.05f, 0.05f, 1.0f };
                switch (character.Status)
                {
                    case CharacterStatus::None:
                        break;
                    case CharacterStatus::InWord:
                        bgColor = Color::Yellow();
                        break;
                    case CharacterStatus::Correct:
                        bgColor = Color::Green();
                        break;
                    case CharacterStatus::Invalid:
                        bgColor = Color::DarkRed();
                        break;
                }

                Renderer->DrawFilledRect(pos, size, bgColor);
                Renderer->DrawRect(pos, size, 1, { 0.5f, 0.5f, 0.5f, 1.0f });

                // get the character as a string view. is this a hack? probably. but it works!
                std::string_view chr(&character.Chr, 1);
                glm::uvec2 textSize = _font->MeasureText(50, chr);
                glm::ivec2 textPos = pos + size / 2 - static_cast<glm::ivec2>(textSize / 2U);
                _font->Draw(textPos, 50, chr, Color::White());
            }
        }
    };
}