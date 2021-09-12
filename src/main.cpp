#include "raylib.h"
#include "graphics.h"
#include "game_logic.h"
#include "models.h"
#include "collisions.h"

#define NDEBUG true

constexpr auto SCREEN_WIDTH = 1600;
constexpr auto SCREEN_HEIGHT = 900;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window title");
    SetTargetFPS(60);
    GraphicsInit();
    CollectablesInit();
    GameLogicInit();
    ObstaclesInit();

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png");

    while (!WindowShouldClose()) {
        GameLogicUpdate();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Draw3DObjects();

#ifndef NDEBUG
        //Debug Text
        DrawText(TextFormat("X: %f", playerPos.x), 3, 21, 20, WHITE);
        DrawText(TextFormat("Y: %f", playerPos.y), 3, 42, 20, WHITE);
        DrawText(TextFormat("Z: %f", playerPos.z), 3, 63, 20, WHITE);
        DrawText(TextFormat("V: %f", velocity), 3, 84, 20, WHITE);
        DrawText(TextFormat("SIDEWAYS V: %f", sidewaysVelocity), 3, 84, 20, GRAY);
        DrawText(TextFormat("TIME TILL TUMBLE: %f", (lastTumbleTime + tumbleInterval) - GetTime() ), 3, 105, 20, GRAY);
#endif
        // UI TEXT
        DrawText(TextFormat("DRUNKNESS : %s", ((drunkTier == SOBER) ? "SOBER" : ((drunkTier == TIPSY) ? "TIPSY" : "NAUSEOUS"))),
                            3, 0, 20,
                            (drunkTier == SOBER) ? GREEN : (drunkTier == TIPSY) ? YELLOW : RED);        
        
        if (drunkTier == NAUSEOUS) {
            DrawText(TextFormat("GET SOBER OR CAUSE SOME CARNAGE"), SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT - 10, 40, DARKPURPLE);
            DrawText(TextFormat("Puke Countdown : %i", static_cast<int>(nauseousCountdown)), SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT - 10, 40, DARKPURPLE);
        }
    
        EndDrawing();
    }

    GraphicsCleanup();
    GameLogicCleanup();
    CloseWindow();

    return 0;
}
