#include "raylib.h"
#include "graphics.h"
#include "game_logic.h"
#include "models.h"
#include "collisions.h"

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

        //Debug Text
        DrawText(TextFormat("X: %f", playerPos.x), 3, 0, 20, WHITE);
        DrawText(TextFormat("Y: %f", playerPos.y), 3, 21, 20, WHITE);
        DrawText(TextFormat("Z: %f", playerPos.z), 3, 42, 20, WHITE);
        DrawText(TextFormat("V: %f", velocity), 3, 63, 20, WHITE);
        DrawText(TextFormat("SIDEWAYS V: %f", sidewaysVelocity), 3, 84, 20, GRAY);
        DrawText(TextFormat("TIME TILL TUMBLE: %f", (lastTumbleTime + tumbleInterval) - GetTime() ), 3, 105, 20, GRAY);

        // UI TEXT
        DrawText(TextFormat("DRUNKNESS : %s", ((drunkTier == SOBER) ? "SOBER" : ((drunkTier == TIPSY) ? "TIPSY" : "NAUSEOUS"))),
                            3, 126, 20,
                            (drunkTier == SOBER) ? GREEN : (drunkTier == TIPSY) ? YELLOW : RED); // todo: change colour green/amber/red based on drunkness
        

        
        EndDrawing();
    }

    GraphicsCleanup();
    GameLogicCleanup();
    CloseWindow();

    return 0;
}
