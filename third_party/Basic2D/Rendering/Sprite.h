#pragma once

constexpr int MAX_SPRITES = 100;
constexpr int MAX_UISPRITES = 100;

struct SpriteData
{
    glm::vec2 Position;
    glm::vec2 OffsiteAtlas;
    glm::vec2 SizeAtlas;
    glm::vec2 Size;
    glm::vec2 Pivot;
    glm::ivec2 AtlasIndexAndFlags;
};

struct UISpriteRender
{
    int amount;
    SpriteData spriteData[MAX_UISPRITES];
};

struct SpriteRender
{
    int amount;
    SpriteData spriteData[MAX_SPRITES];
};

static SpriteRender spriteRender;
static UISpriteRender uiSpriteRender;

void ResetSpriteRender()
{
    spriteRender.amount = 0;
    uiSpriteRender.amount = 0;
}

void DrawSprite(SpriteAtlasId spriteId, glm::vec2 position, glm::vec2 size, glm::vec2 pivot = glm::vec2(0.0f, 0.0f))
{
    if (spriteRender.amount >= MAX_SPRITES)
        return;

    AtlasCordinat spriteSet = GetSprite(spriteId);

    SpriteData &data = spriteRender.spriteData[spriteRender.amount];
    data.Position = position;
    data.OffsiteAtlas = spriteSet.OffsiteAtlasSprite;
    data.SizeAtlas = spriteSet.SizeAtlasSprite;
    data.Size = size;
    data.Pivot = pivot;
    data.AtlasIndexAndFlags = {spriteSet.atlasIndex, 0};  

    spriteRender.amount++;
}

void DrawUISprite(SpriteAtlasId spriteId, glm::vec2 position, glm::vec2 size, glm::vec2 pivot = glm::vec2(0.0f, 0.0f))
{
    AtlasCordinat spriteSet = GetSprite(spriteId);

    SpriteData &data = uiSpriteRender.spriteData[uiSpriteRender.amount];
    data.Position = position;
    data.OffsiteAtlas = spriteSet.OffsiteAtlasSprite;
    data.SizeAtlas = spriteSet.SizeAtlasSprite;
    data.Size = size;
    data.Pivot = pivot;
    data.AtlasIndexAndFlags = {spriteSet.atlasIndex, 0};

    uiSpriteRender.amount++;
}

void DrawAnimatedSprite(SpriteAtlasId spriteId, glm::vec2 position, glm::vec2 size, glm::vec2 pivot = glm::vec2(0.0f, 0.0f))
{
    if (spriteRender.amount >= MAX_SPRITES)
        return;

    SpriteAnimation anim = GetSpriteAnimation(spriteId);
    if (anim.frameCount <= 0)
        return;

    float totalTime = GetTotalTime();
    int currentFrame = (int)(totalTime / anim.frameDuration) % anim.frameCount;

    AtlasCordinat spriteSet = GetSprite(spriteId, currentFrame);

    SpriteData &data = spriteRender.spriteData[spriteRender.amount];
    data.Position = position;
    data.OffsiteAtlas = spriteSet.OffsiteAtlasSprite;
    data.SizeAtlas = spriteSet.SizeAtlasSprite;
    data.Size = size;
    data.Pivot = pivot;
    data.AtlasIndexAndFlags = {spriteSet.atlasIndex, 0};

    spriteRender.amount++;
}