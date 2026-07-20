#include <Basic2D/Engine.h>

enum class FacingDir
{
    Up,
    Down,
    Left,
    Right
};

struct PlayerState
{
    glm::vec2 playerPos = {0.0f, 100.0f};
    glm::vec2 lastPos = {0.0f, 0.0f};
    float diceSpeed = 200.0f;
    SpriteAtlasId activeSprite;
    FacingDir facing = FacingDir::Down;

    glm::vec2 colliderOffset = {0.0f, 50.0f};
    glm::vec2 colliderSize = {20.0f, 20.0f};
};

static PlayerState state;

inline glm::vec2 GetColliderPos()
{
    return state.playerPos + state.colliderOffset;
}

void Player()
{
    float dt = GetDeltaTime();
    state.lastPos = state.playerPos;

    glm::vec2 movement = GetVector(ActionCode::A, ActionCode::D, ActionCode::S, ActionCode::W);
    bool isMoving = (movement.x != 0.0f || movement.y != 0.0f);

    if (isMoving)
    {
        glm::vec2 velocity = movement * state.diceSpeed * dt;
        glm::vec2 colliderPos = GetColliderPos();

        if (velocity.x != 0.0f)
        {
            glm::vec2 newColliderPos = colliderPos + glm::vec2(velocity.x, 0.0f);
            if (!IsPositionBlocked(newColliderPos, state.colliderSize, CollisionType::circle))
            {
                state.playerPos.x += velocity.x;
            }
        }

        if (velocity.y != 0.0f)
        {
            glm::vec2 newColliderPos = GetColliderPos() + glm::vec2(0.0f, velocity.y);
            if (!IsPositionBlocked(newColliderPos, state.colliderSize, CollisionType::circle))
            {
                state.playerPos.y += velocity.y;
            }
        }

        if (fabsf(movement.x) > fabsf(movement.y))
        {
            state.facing = (movement.x < 0.0f) ? FacingDir::Left : FacingDir::Right;
        }
        else
        {
            state.facing = (movement.y < 0.0f) ? FacingDir::Up : FacingDir::Down;
        }
    }

    if (isMoving)
    {
        switch (state.facing)
        {
        case FacingDir::Up:
            state.activeSprite = SPRITE_PLAYER_WALK_UP;
            break;
        case FacingDir::Down:
            state.activeSprite = SPRITE_PLAYER_WALK_DOWN;
            break;
        case FacingDir::Left:
        case FacingDir::Right:
            state.activeSprite = SPRITE_PLAYER_WALK_MIRRO;
            break;
        }
    }
    else
    {
        switch (state.facing)
        {
        case FacingDir::Up:
            state.activeSprite = SPRITE_PLAYER_IDLE_UP;
            break;
        case FacingDir::Down:
            state.activeSprite = SPRITE_PLAYER_IDLE_DOWN;
            break;
        case FacingDir::Left:
        case FacingDir::Right:
            state.activeSprite = SPRITE_PLAYER_IDLE_MIRRO;
            break;
        }
    }

    glm::vec2 spriteSize = {128.0f, 128.0f};
    if (state.facing == FacingDir::Left)
        spriteSize.x = -spriteSize.x;

    DrawAnimatedSprite(state.activeSprite, state.playerPos, spriteSize, {0.5f, 0.5f});
    
    RegisterWorldCollider(GetColliderPos(), state.colliderSize, circle, ColliderCategory::Anchor);

    glm::vec2 screenHalf = renderWindows.ScreenSize * 0.5f;
    gameCam.pos = state.playerPos - screenHalf;
}