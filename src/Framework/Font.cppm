module;

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include <glm/glm.hpp>

#include <string>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <cassert>
#include <vector>
#include <memory>
#include <unordered_map>

export module Birdle.Framework:Font;
import :Renderer;

export namespace bdl
{
    class Font final
    {
        struct Character
        {
            Texture& Atlas; // atlas texture containing the character
            glm::ivec4 Source; // source rectangle
            glm::ivec2 Bearing; // horiz + vert bearing
            int32_t Advance; // horizontal advance
        };

        static constexpr glm::uvec2 TextureSize = { 1024, 1024 };

        Renderer& _renderer;

        uint8_t* _fontBuf;
        stbtt_fontinfo _font;
        std::unordered_map<uint64_t, Character> _characters; // map of character + size to character info
        std::vector<std::unique_ptr<Texture>> _atlases; // font atlases. supports multiple atlases for large amounts of characters

        size_t _currentAtlas{}; // the current atlas texture index to write new characters to
        glm::uvec2 _currentPos{}; // the position in the current atlas texture to write a new character to
        uint32_t _largestHeightOnCurrentLine{}; // the largest character height on the current line

    public:
        explicit Font(Renderer& renderer, const std::string& path) : _renderer(renderer)
        {
            FILE* f = fopen(path.c_str(), "rb");
            if (!f)
                throw std::runtime_error("Failed to fopen");

            fseek(f, 0, SEEK_END);
            size_t size = ftell(f);
            fseek(f, 0, SEEK_SET);

            // load font into buffer
            _fontBuf = new uint8_t[size];
            size_t read = fread(_fontBuf, 1, size, f);
            assert(read == size);

            if (!stbtt_InitFont(&_font, _fontBuf, 0))
                throw std::runtime_error("Failed to initialize font!");

            _atlases.push_back(_renderer.CreateTexture(nullptr, TextureSize, PixelFormat::RGBA8));
        }

        ~Font()
        {
            delete[] _fontBuf;
        }

        void Draw(const glm::vec2& pos, uint32_t size, const std::string_view& text, const Color& color)
        {
            int ascent, descent;
            stbtt_GetFontVMetrics(&_font, &ascent, &descent, nullptr);
            float scale = stbtt_ScaleForPixelHeight(&_font, size);

            glm::vec2 currentPos = pos;
            for (const char c : text)
            {
                switch (c)
                {
                    case '\n':
                    {
                        currentPos.x = pos.x;
                        currentPos.y += ascent * scale;
                        continue;
                    }
                }

                Character character = GetCharacter(size, c, scale);
                _renderer.DrawSprite(Sprite(character.Atlas,
                    currentPos + glm::vec2(character.Bearing.x, character.Bearing.y + (ascent * scale) + (descent * scale)),
                    character.Source, color));
                currentPos.x += character.Advance;
            }
        }

    private:
        Character GetCharacter(uint32_t size, uint32_t c, float scale)
        {
            // compute the hash
            // the hash is the a combination of the font size and codepoint
            uint64_t hash = (static_cast<uint64_t>(size) << 32) | c;

            // find existing character
            auto chr = _characters.find(hash);
            if (chr != _characters.end())
                return chr->second;

            int w, h, left, top;
            unsigned char* bitmap = stbtt_GetCodepointBitmap(&_font, 0, scale, c, &w, &h, &left, &top);

            if (h >= _largestHeightOnCurrentLine)
                _largestHeightOnCurrentLine = h;

            // convert bitmap to RGBA
            std::vector<uint8_t> rgbaBitmap;
            rgbaBitmap.reserve(w * h * 4);
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    rgbaBitmap.emplace_back(255);
                    rgbaBitmap.emplace_back(255);
                    rgbaBitmap.emplace_back(255);
                    rgbaBitmap.emplace_back(bitmap[(y * w + x)]);
                }
            }

            // ensure the character can fit in the current texture
            if (_currentPos.x + w >= TextureSize.x)
            {
                _currentPos.x = 0;
                _currentPos.y += _largestHeightOnCurrentLine;
                _largestHeightOnCurrentLine = 0;

                // no more room in the current atlas!
                // create a new one.
                if (_currentPos.y + h >= TextureSize.y)
                {
                    _currentPos = {};
                    auto texture = _renderer.CreateTexture(nullptr, TextureSize, PixelFormat::RGBA8);
                    _atlases.push_back(std::move(texture));
                    _currentAtlas++;
                }
            }

            _atlases[_currentAtlas]->Update(_currentPos, glm::uvec2(w, h), rgbaBitmap.data());
            stbtt_FreeBitmap(bitmap, nullptr);

            int advance, bearingX;
            stbtt_GetCodepointHMetrics(&_font, c, &advance, &bearingX);

            Character character
            {
                .Atlas = *_atlases[_currentAtlas],
                .Source = glm::ivec4(_currentPos, w, h),
                .Bearing = glm::ivec2(left, top),
                .Advance = static_cast<int32_t>(advance * scale)
            };

            _currentPos.x += w;
            _characters.emplace(hash, character);
            return character;
        }
    };
}