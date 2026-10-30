module;

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <stdexcept>

export module Birdle.Framework:Texture;

export namespace bdl
{
    enum class PixelFormat
    {
        RGBA8
    };

    class Texture final
    {
        friend class Renderer;

        GLuint _handle;
        glm::ivec2 _size;

        explicit Texture(void* data, glm::ivec2 size, PixelFormat format)
        {
            _size = size;

            GLint internalFormat;
            GLenum pixelFormat;
            GLenum dataType;

            switch (format)
            {
                case PixelFormat::RGBA8:
                    internalFormat = GL_RGBA8;
                    pixelFormat = GL_RGBA;
                    dataType = GL_UNSIGNED_BYTE;
                    break;
                default:
                    throw std::runtime_error("Unknown pixel format.");
            }

            glGenTextures(1, &_handle);
            glBindTexture(GL_TEXTURE_2D, _handle);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, pixelFormat, dataType, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINE);
        }

    public:
        ~Texture()
        {
            glDeleteTextures(1, &_handle);
        }

        [[nodiscard]] inline GLuint Handle() const { return _handle; }

        [[nodiscard]] inline glm::ivec2 Size() const { return _size; }
    };
}