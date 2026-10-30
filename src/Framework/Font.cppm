module;

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <string>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <cassert>

export module Birdle.Framework:Font;
import :Renderer;

export namespace bdl
{
    class Font final
    {
        uint8_t* _fontBuf;
        stbtt_fontinfo _font;
    public:
        explicit Font(const std::string& path)
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
        }

        ~Font()
        {
            delete[] _fontBuf;
        }
    };
}