#include <Basic2D/Engine.h>
#include "conten/Player.cpp"
#include "map.cpp"

static const ColliderDef s_LevelColliders[] = {
    {{68.5f, 55.5f}, {659.0f, 7.0f}, square, ColliderCategory::Static, 0.0f},
};

void rendy_on_game()
{
}

void update_game()
{
    Ruler_Update();

    RegisterWorldCollidersFromArray(s_LevelColliders, 1);

    DrawSprite(SPRITE_MAP_WATER_BACKGROUND, {0.0f, -300.0f}, {5 * 1024.0f, 5 * 520.0f}, {0.5f, 0.0f});

    DrawSprite(SPRITE_MAP, {0.0f, 0.0f}, {3 * 1024.0f, 3 * 520.0f}, {0.5f, 0.0f});

    Player();
}

void update_ui()
{
    static float cpuDisplay = 0.0f;
    static float timer = 0.0f;
    timer += GetDeltaTime();

    if (timer >= 0.5f)
    {
        cpuDisplay = GetProcessCPUUsagePercent();
        timer = 0.0f;
    }

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "CPU: %.1f%%", cpuDisplay);
    DrawText(buffer, {20.0f, 20.0f});
    // DrawUISprite(SPRITE_PLAYER_IDLE_DOWN, {100.0f, 100.0f}, {100.0f, 100.0f}, {0.5f, 0.5f});
}