#pragma once

#include <Basic2D/Engine.h>

enum SpriteAtlasId
{
    SPRITE_MAP,
    SPRITE_MAP_WATER_BACKGROUND,
    SPRITE_PLAYER_IDLE_DOWN,
    SPRITE_PLAYER_WALK_MIRRO,
    SPRITE_PLAYER_IDLE_MIRRO,
    SPRITE_PLAYER_IDLE_UP,
    SPRITE_PLAYER_WALK_DOWN,
    SPRITE_PLAYER_WALK_UP,
};

enum AtlasId
{
    ATLAS_ONE,
    ATLAS_TWO,
    ATLAS_TREE,

    ATLAS_COUNT
};

static const char *AtlasFilePaths[ATLAS_COUNT] = {
    "assets/texture/texture_atlas.png",
    "assets/texture/texture_atlas_two.png",
    "assets/texture/texture_atlas_tree.png",
};

struct AtlasCordinat
{
    glm::vec2 OffsiteAtlasSprite;
    glm::vec2 SizeAtlasSprite;
    int atlasIndex;
};

struct SpriteAnimation
{
    int frameCount;
    float frameDuration;
};

AtlasCordinat GetSprite(SpriteAtlasId spriteId, int frameIndex = 0)
{
    AtlasCordinat result = {};
    result.atlasIndex = ATLAS_ONE;

    switch (spriteId)
    {
    case SPRITE_MAP:
        result.atlasIndex = ATLAS_TWO;
        result.OffsiteAtlasSprite = {0.0f, 0.0f};
        result.SizeAtlasSprite = {1024.0f, 520.0f};
        break;

    case SPRITE_MAP_WATER_BACKGROUND:
        result.atlasIndex = ATLAS_TREE;
        result.OffsiteAtlasSprite = {0.0f, 0.0f};
        result.SizeAtlasSprite = {1024.0f, 520.0f};
        break;

    case SPRITE_PLAYER_IDLE_DOWN:
        switch (frameIndex)
        {
        case 0:
            result.OffsiteAtlasSprite = {16.0f, 0.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 1:
            result.OffsiteAtlasSprite = {80.0f, 0.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 2:
            result.OffsiteAtlasSprite = {144.0f, 0.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 3:
            result.OffsiteAtlasSprite = {208.0f, 0.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 4:
            result.OffsiteAtlasSprite = {272.0f, 0.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 5:
            result.OffsiteAtlasSprite = {336.0f, 0.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        default:
            result.OffsiteAtlasSprite = {16.0f, 0.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        }
        break;

    case SPRITE_PLAYER_WALK_DOWN:
        switch (frameIndex)
        {
        case 0:
            result.OffsiteAtlasSprite = {16.0f, 64.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 1:
            result.OffsiteAtlasSprite = {80.0f, 64.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 2:
            result.OffsiteAtlasSprite = {144.0f, 64.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 3:
            result.OffsiteAtlasSprite = {208.0f, 64.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 4:
            result.OffsiteAtlasSprite = {272.0f, 64.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 5:
            result.OffsiteAtlasSprite = {336.0f, 64.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        default:
            result.OffsiteAtlasSprite = {16.0f, 64.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        }
        break;

    case SPRITE_PLAYER_WALK_MIRRO:
        switch (frameIndex)
        {
        case 0:
            result.OffsiteAtlasSprite = {16.0f, 320.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 1:
            result.OffsiteAtlasSprite = {80.0f, 320.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 2:
            result.OffsiteAtlasSprite = {144.0f, 320.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 3:
            result.OffsiteAtlasSprite = {208.0f, 320.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 4:
            result.OffsiteAtlasSprite = {272.0f, 320.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 5:
            result.OffsiteAtlasSprite = {336.0f, 320.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        default:
            result.OffsiteAtlasSprite = {16.0f, 320.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        }
        break;

    case SPRITE_PLAYER_WALK_UP:
        switch (frameIndex)
        {
        case 0:
            result.OffsiteAtlasSprite = {16.0f, 128.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 1:
            result.OffsiteAtlasSprite = {80.0f, 128.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 2:
            result.OffsiteAtlasSprite = {144.0f, 128.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 3:
            result.OffsiteAtlasSprite = {208.0f, 128.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 4:
            result.OffsiteAtlasSprite = {272.0f, 128.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 5:
            result.OffsiteAtlasSprite = {336.0f, 128.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        default:
            result.OffsiteAtlasSprite = {16.0f, 128.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        }
        break;

    case SPRITE_PLAYER_IDLE_MIRRO:
        switch (frameIndex)
        {
        case 0:
            result.OffsiteAtlasSprite = {16.0f, 192.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 1:
            result.OffsiteAtlasSprite = {80.0f, 192.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 2:
            result.OffsiteAtlasSprite = {144.0f, 192.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 3:
            result.OffsiteAtlasSprite = {208.0f, 192.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 4:
            result.OffsiteAtlasSprite = {272.0f, 192.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 5:
            result.OffsiteAtlasSprite = {336.0f, 192.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        default:
            result.OffsiteAtlasSprite = {16.0f, 192.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        }
        break;

    case SPRITE_PLAYER_IDLE_UP:
        switch (frameIndex)
        {
        case 0:
            result.OffsiteAtlasSprite = {16.0f, 256.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 1:
            result.OffsiteAtlasSprite = {80.0f, 256.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 2:
            result.OffsiteAtlasSprite = {144.0f, 256.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 3:
            result.OffsiteAtlasSprite = {208.0f, 256.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 4:
            result.OffsiteAtlasSprite = {272.0f, 256.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        case 5:
            result.OffsiteAtlasSprite = {336.0f, 256.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        default:
            result.OffsiteAtlasSprite = {16.0f, 256.0f};
            result.SizeAtlasSprite = {64.0f, 64.0f};
            break;
        }
        break;

    default:
        LOG_ERROR("GetSprite: SpriteAtlasId %d tidak dikenal", (int)spriteId);
        break;
    }

    return result;
}

SpriteAnimation GetSpriteAnimation(SpriteAtlasId spriteId)
{
    SpriteAnimation anim = {};

    switch (spriteId)
    {
    case SPRITE_PLAYER_IDLE_DOWN:
        anim.frameCount = 6;
        anim.frameDuration = 0.15f;
        break;
    case SPRITE_PLAYER_WALK_MIRRO:
        anim.frameCount = 6;
        anim.frameDuration = 0.15f;
        break;
    case SPRITE_PLAYER_WALK_DOWN:
        anim.frameCount = 6;
        anim.frameDuration = 0.15f;
        break;
    case SPRITE_PLAYER_WALK_UP:
        anim.frameCount = 6;
        anim.frameDuration = 0.15f;
        break;
    case SPRITE_PLAYER_IDLE_MIRRO:
        anim.frameCount = 6;
        anim.frameDuration = 0.15f;
        break;
    case SPRITE_PLAYER_IDLE_UP:
        anim.frameCount = 6;
        anim.frameDuration = 0.15f;
        break;

    default:
        LOG_ERROR("GetSpriteAnimation: SpriteAtlasId %d tidak punya animasi", (int)spriteId);
        break;
    }

    return anim;
}