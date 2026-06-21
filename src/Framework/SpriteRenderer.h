#pragma once

#include "BufferShaderSet.h"

#include <glm/glm.hpp>

#include <array>
#include <memory>

namespace birdle
{
    class SpriteRenderer final
    {
        struct Vertex
        {
            glm::vec2 position;
            glm::vec2 texCoord;
            glm::vec4 tint;
        };
        
        static constexpr uint32_t MAX_SPRITES_PER_BATCH = 4096;
        
        static constexpr uint32_t NUM_VERTICES = 4;
        static constexpr uint32_t NUM_INDICES = 6;
        
        static constexpr uint32_t MAX_VERTICES = NUM_VERTICES * MAX_SPRITES_PER_BATCH;
        static constexpr uint32_t MAX_INDICES = NUM_INDICES * MAX_SPRITES_PER_BATCH;
        
        std::array<Vertex, MAX_VERTICES> _vertices{};
        std::array<uint32_t, MAX_INDICES> _indices{};
        
        std::unique_ptr<BufferShaderSet> _set{};
        
    public:
        SpriteRenderer();
    };
}
