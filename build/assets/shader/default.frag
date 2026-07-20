#version 430 core

uniform sampler2DArray AtlasTexture;

in vec2 CordinatAtlas;
flat in int AtlasIndex;

out vec4 FragColor;

void main()
{
    vec2 atlasSize = vec2(textureSize(AtlasTexture, 0).xy);
    vec2 uv = CordinatAtlas / atlasSize;
    vec4 texColor = texture(AtlasTexture, vec3(uv, float(AtlasIndex)));

    if (texColor.a < 0.01)
    {
        discard;
    }

    FragColor = texColor;
}