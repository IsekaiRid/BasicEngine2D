#include <Basic2D/Engine.h>
#include "conten/Player.cpp"
#include "map.cpp"

static const ColliderDef s_LevelColliders[] = {

};

void rendy_on_game()
{
}

void update_game()
{
    Ruler_Update();

    // RegisterWorldCollidersFromArray(s_LevelColliders, 3);

    DrawSprite(SPRITE_MAP_WATER_BACKGROUND, {0.0f, -300.0f}, {5 * 1024.0f, 5 * 520.0f}, {0.5f, 0.0f});

    DrawSprite(SPRITE_MAP, {0.0f, 0.0f}, {3 * 1024.0f, 3 * 520.0f}, {0.5f, 0.0f});

    Player();
}

void update_ui()
{
    // DrawUISprite(SPRITE_PLAYER_IDLE_DOWN, {100.0f, 100.0f}, {100.0f, 100.0f}, {0.5f, 0.5f});
}