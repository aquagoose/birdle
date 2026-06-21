#include "SpriteRenderer.h"

// vertex shader
const char* SPRITE_VSHADER_SRC = R"(
#version 330 core

in vec2 aPosition;
in vec2 aTexCoord;
in vec4 aTint;

out vec2 frag_TexCoord;
out vec4 frag_Tint;

uniform mat4 uProjection;
uniform mat4 uTransform;

void main()
{
    gl_Position = uProjection * uTransform * vec4(aPosition, 0.0, 1.0);
    frag_TexCoord = aTexCoord;
    frag_Tint = aTint;
}
)";

// fragment shader
const char* SPRITE_FSHADER_SRC = R"(
#version 330 core

in vec2 frag_TexCoord;
in vec4 frag_Tint;

out vec4 out_Color;

uniform sampler2D uSprite;

void main()
{
    out_Color = texture(uSprite, frag_TexCoord) * frag_Tint;
}
)";

namespace birdle
{
    SpriteRenderer::SpriteRenderer()
    {
        _set = std::make_unique<BufferShaderSet>(MAX_VERTICES, MAX_INDICES, SPRITE_VSHADER_SRC, SPRITE_FSHADER_SRC);
        
        InputElement inputLayout[] = {
            {
                .name = "aPosition",
                .type = InputElementType::Float2,
                .offset = offsetof(Vertex, position)
            },
            {
                .name = "aTexCoord",
                .type = InputElementType::Float2,
                .offset = offsetof(Vertex, texCoord)
            },
            {
                .name = "aTint",
                .type = InputElementType::Float4,
                .offset = offsetof(Vertex, tint)
            }
        };
        _set->setup_shader_attribs(sizeof(Vertex), inputLayout);
    }
}
