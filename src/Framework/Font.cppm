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
#include <iostream>

export module Birdle.Framework:Font;
import :Renderer;

export namespace bdl
{
    class Font final
    {
        struct Character
        {
            size_t Texture;
            glm::ivec4 Source;
            glm::ivec2 Bearing;
            uint32_t Advance;
        };

        Renderer& _renderer;

        uint8_t* _fontBuf;
        stbtt_fontinfo _font;
        std::unordered_map<uint64_t, Character> _characters;
        std::vector<std::unique_ptr<Texture>> _textures;

        glm::ivec4 _currentSource;

    public:
        explicit Font(Renderer& renderer, const std::string& path) : _renderer(renderer)
        {
            FILE* f = fopen(path.c_str(), "rb");
            if (!f)
                throw std::runtime_error("Failed to fopen");

            fseek(f, 0, SEEK_END);
            size_t size = ftell(f);
            fseek(f, 0, SEEK_SET);

            _fontBuf = new uint8_t[size];
            size_t read = fread(_fontBuf, 1, size, f);
            assert(read == size);

            if (!stbtt_InitFont(&_font, _fontBuf, 0))
                throw std::runtime_error("Failed to initialize font!");

            _textures.push_back(_renderer.CreateTexture(nullptr, glm::ivec2(1024, 1024), PixelFormat::RGBA8));
        }

        ~Font()
        {
            delete[] _fontBuf;
        }

        void Draw(uint32_t size, const std::string& text)
        {
            Character character = GetCharacter(size, 'A');
            _renderer.DrawSprite(Sprite(*_textures[0], glm::vec2(0, 0)));
        }

    private:
        Character GetCharacter(uint32_t size, uint32_t c)
        {
            auto chr = _characters.find(size | c);
            if (chr != _characters.end())
                return chr->second;

            std::cout << "Creating character" << std::endl;
            int w, h;

            unsigned char* bitmap = stbtt_GetCodepointBitmap(&_font, 0, stbtt_ScaleForPixelHeight(&_font, size), c, &w, &h, nullptr, nullptr);
            uint8_t* rgbaBitmap = new uint8_t[w * h * 4];
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    rgbaBitmap[4 * (y * w + x) + 0] = 255;
                    rgbaBitmap[4 * (y * w + x) + 1] = 255;
                    rgbaBitmap[4 * (y * w + x) + 2] = 255;
                    rgbaBitmap[4 * (y * w + x) + 3] = bitmap[(y * w + x)];
                }
            }

            _textures[0]->Update(glm::ivec2(0, 0), glm::uvec2(w, h), rgbaBitmap);
            delete[] rgbaBitmap;
            stbtt_FreeBitmap(bitmap, nullptr);

            Character character
            {
                .Texture = 0,
                .Source = glm::ivec4(0, 0, w, h),
                .Bearing = glm::ivec2(0, 0),
                .Advance = 0
            };

            _characters.emplace(size | c, character);
            return character;
        }
    };
}