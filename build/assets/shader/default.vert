#version 430 core

layout (location = 0) uniform vec2 ScreenSize;
uniform mat4 ViewProjection;

struct SpriteData
{
    vec2 Position;
    vec2 OffsiteAtlas;
    vec2 SizeAtlas;
    vec2 Size;
    vec2 Pivot;          
    ivec2 AtlasIndexAndFlags;
};

layout(std430, binding = 0) buffer SpriteBuffer
{
    SpriteData sprites[];
};

const vec2 localVerts[4] = vec2[](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0),
    vec2(0.0, 1.0)
);

const int indices[6] = int[](
    0, 1, 3,
    1, 2, 3
);

out vec2 CordinatAtlas;
flat out int AtlasIndex;

void main()
{
    SpriteData sprite = sprites[gl_InstanceID];
    vec2 local = localVerts[indices[gl_VertexID]];

    vec2 worldPos = sprite.Position + (local - sprite.Pivot) * sprite.Size;

    gl_Position = ViewProjection * vec4(worldPos, 0.0, 1.0);

    CordinatAtlas = sprite.OffsiteAtlas + local * sprite.SizeAtlas;
    AtlasIndex = sprite.AtlasIndexAndFlags.x;
}