#pragma once

#include <Basic2D/Core/Assets.h>
#include <glm/glm.hpp>

struct ShaderProgram
{
    unsigned int vertexShaderId;
    unsigned int fragmentShaderId;
    unsigned int programId;
};

struct AtlasData
{
    unsigned int textureArrayId;
    glm::vec2 sizes[ATLAS_COUNT];
};

struct SpriteRenderer
{
    unsigned int VAO;
    unsigned int spriteSSBO;
    unsigned int uiSpriteSSBO;
    unsigned int screenSizeLoc;
};

struct CollisionRenderer
{
    unsigned int collisionDataSSBO;
    unsigned int VAO;
    ShaderProgram program;
};

struct ShaderData
{
    ShaderProgram program;
    AtlasData atlas;
    SpriteRenderer sprite;
    CollisionRenderer collision;
};

static ShaderData shaderData;
