#pragma once

#include <Basic2D/Core/Config.h>
#include <Basic2D/Core/File.h>
#include <Basic2D/Core/Logger.h>
#include <Basic2D/Core/Camera2D.h>
#include <Basic2D/Rendering/StructRender.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cmath>

constexpr int WORLD_COLLIDER_MAX = 100;

enum CollisionType
{
    square,
    circle,
};

enum class ColliderCategory
{
    Static,
    Anchor,
    Area,
};

struct CollisionRenderData
{
    glm::vec2 Position; // offset 0
    glm::vec2 Size;     // offset 8
    float Rotation;     // offset 16
    int Type;           // offset 20
    float _padding[2];  // offset 24..31 -> pad to 32
    glm::vec4 Color;    // offset 32..47
};

static_assert(sizeof(CollisionRenderData) == 48, "CollisionRenderData must be 48 bytes for std430");

struct WorldCollider
{
    glm::vec2 Position;
    glm::vec2 HalfSize;
    float Rotation;
    CollisionType Type;
    ColliderCategory Category;
};

struct WorldColliderContainer
{
    int amount = 0;
    WorldCollider colliders[WORLD_COLLIDER_MAX];
    bool DebugActive = false;
};

// === Collider Definition untuk array-based registration ===
struct ColliderDef
{
    glm::vec2 Position;
    glm::vec2 Size;
    CollisionType Type;
    ColliderCategory Category;
    float Rotation;
};

static WorldColliderContainer worldColliders;

static inline bool CheckCircleCircleOverlap(glm::vec2 aPos, float aRadius,
                                            glm::vec2 bPos, float bRadius);

static inline bool CheckAABBCircleOverlap(glm::vec2 aPos, glm::vec2 aHalf,
                                          glm::vec2 cPos, float cRadius);

void Collision_Debug_Enable() { worldColliders.DebugActive = true; }
void Collision_Debug_Disable() { worldColliders.DebugActive = false; }
void Collision_Debug_Toggle() { worldColliders.DebugActive = !worldColliders.DebugActive; }

void ClearWorldColliders()
{
    worldColliders.amount = 0;
}

void RegisterWorldCollider(glm::vec2 position, glm::vec2 size, CollisionType type,
                           ColliderCategory category = ColliderCategory::Static, float rotation = 0.0f)
{
    if (worldColliders.amount >= WORLD_COLLIDER_MAX)
    {
        LOG_ERROR("World collider penuh (max: %d)", WORLD_COLLIDER_MAX);
        return;
    }

    worldColliders.colliders[worldColliders.amount++] = {position, size * 0.5f, rotation, type, category};
}

// === BARU: Register dari array ColliderDef ===
void RegisterWorldCollidersFromArray(const ColliderDef *defs, int count)
{
    for (int i = 0; i < count; i++)
    {
        const ColliderDef &d = defs[i];
        RegisterWorldCollider(d.Position, d.Size, d.Type, d.Category, d.Rotation);
    }
}

// === BARU: Macro helper biar nggak perlu hitung count manual ===
#define COLLIDER_ARRAY(...) \
    RegisterWorldCollidersFromArray(__VA_ARGS__, sizeof(__VA_ARGS__) / sizeof(ColliderDef))

static inline bool CheckAABBOverlap(glm::vec2 posA, glm::vec2 halfA, glm::vec2 posB, glm::vec2 halfB)
{
    return (fabsf(posA.x - posB.x) < (halfA.x + halfB.x)) &&
           (fabsf(posA.y - posB.y) < (halfA.y + halfB.y));
}

bool IsPositionBlocked(glm::vec2 testPos, glm::vec2 size, CollisionType testType = square)
{
    glm::vec2 halfSize = size * 0.5f;
    float testRadius = (testType == circle) ? halfSize.x : 0.0f;

    for (int i = 0; i < worldColliders.amount; i++)
    {
        const WorldCollider &w = worldColliders.colliders[i];

        if (w.Category == ColliderCategory::Anchor)
            continue;

        if (testType == square && w.Type == square)
        {
            if (CheckAABBOverlap(testPos, halfSize, w.Position, w.HalfSize))
                return true;
        }
        else if (testType == square && w.Type == circle)
        {
            if (CheckAABBCircleOverlap(testPos, halfSize, w.Position, w.HalfSize.x))
                return true;
        }
        else if (testType == circle && w.Type == square)
        {
            if (CheckAABBCircleOverlap(w.Position, w.HalfSize, testPos, testRadius))
                return true;
        }
        else
        {
            if (CheckCircleCircleOverlap(testPos, testRadius, w.Position, w.HalfSize.x))
                return true;
        }
    }
    return false;
}

static inline glm::vec4 GetColliderDebugColor(ColliderCategory category)
{
    (void)category;
    return glm::vec4(0.0f, 0.0f, 1.0f, 0.4f);
}

static inline bool CheckCircleCircleOverlap(glm::vec2 aPos, float aRadius,
                                            glm::vec2 bPos, float bRadius)
{
    float dx = aPos.x - bPos.x;
    float dy = aPos.y - bPos.y;
    float r = aRadius + bRadius;
    return (dx * dx + dy * dy) < (r * r);
}

static inline bool CheckAABBCircleOverlap(glm::vec2 aPos, glm::vec2 aHalf,
                                          glm::vec2 cPos, float cRadius)
{
    glm::vec2 diff = cPos - aPos;
    glm::vec2 clamped = glm::clamp(diff, -aHalf, aHalf);
    glm::vec2 closest = aPos + clamped;
    glm::vec2 v = cPos - closest;
    return (v.x * v.x + v.y * v.y) < (cRadius * cRadius);
}

void Collision_Shader_Init(const char *FilePathVertex, const char *FilePathFragman)
{
    int success;
    char infoLog[512];
    static char ContainerBuffer[MB(2)];

    size_t sizeVertex = FileSize(FilePathVertex);
    char *vertexSource = FileRead(FilePathVertex, ContainerBuffer, sizeVertex);
    shaderData.collision.program.vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderData.collision.program.vertexShaderId, 1, &vertexSource, nullptr);
    glCompileShader(shaderData.collision.program.vertexShaderId);

    glGetShaderiv(shaderData.collision.program.vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderData.collision.program.vertexShaderId, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Vertex Shader Compilation Failed:\n%s", infoLog);
    }

    size_t sizeFragment = FileSize(FilePathFragman);
    char *fragmentSource = FileRead(FilePathFragman, ContainerBuffer, sizeFragment);
    shaderData.collision.program.fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderData.collision.program.fragmentShaderId, 1, &fragmentSource, nullptr);
    glCompileShader(shaderData.collision.program.fragmentShaderId);

    glGetShaderiv(shaderData.collision.program.fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderData.collision.program.fragmentShaderId, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Fragment Shader Compilation Failed:\n%s", infoLog);
    }

    shaderData.collision.program.programId = glCreateProgram();
    glAttachShader(shaderData.collision.program.programId, shaderData.collision.program.vertexShaderId);
    glAttachShader(shaderData.collision.program.programId, shaderData.collision.program.fragmentShaderId);
    glLinkProgram(shaderData.collision.program.programId);

    glDeleteShader(shaderData.collision.program.vertexShaderId);
    glDeleteShader(shaderData.collision.program.fragmentShaderId);

    glGenVertexArrays(1, &shaderData.collision.VAO);
    glGenBuffers(1, &shaderData.collision.collisionDataSSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.collision.collisionDataSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(CollisionRenderData) * WORLD_COLLIDER_MAX, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderData.collision.collisionDataSSBO);
}

void render_Collision_update()
{
    if (!worldColliders.DebugActive || worldColliders.amount == 0)
    {
        return;
    }

    CollisionRenderData renderData[WORLD_COLLIDER_MAX];
    for (int i = 0; i < worldColliders.amount; i++)
    {
        const WorldCollider &c = worldColliders.colliders[i];
        renderData[i].Position = c.Position;
        renderData[i].Size = c.HalfSize * 2.0f;
        renderData[i].Rotation = c.Rotation;
        renderData[i].Type = (int)c.Type;
        renderData[i].Color = GetColliderDebugColor(c.Category);
    }

    glUseProgram(shaderData.collision.program.programId);
    glBindVertexArray(shaderData.collision.VAO);
    GetCameraViewProjection(gameCam, shaderData.collision.program.programId);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.collision.collisionDataSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(CollisionRenderData) * worldColliders.amount, renderData);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderData.collision.collisionDataSSBO);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, worldColliders.amount);
}