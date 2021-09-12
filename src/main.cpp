#include "raylib.h"
#include "graphics.h"
#include "game_logic.h"
#include "models.h"
#include "collisions.h"

#define NDEBUG

constexpr auto SCREEN_WIDTH = 1600;
constexpr auto SCREEN_HEIGHT = 900;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Drunk & Punk");
    SetTargetFPS(60);
    GraphicsInit();
    CollectablesInit();
    GameLogicInit();
    ObstaclesInit();

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png");

    while (!WindowShouldClose()) {
        while (gameLogicLocked)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                ResetGameState();
                gameLogicLocked = false;
            }



            BeginDrawing();
            ClearBackground(RAYWHITE);
            Draw3DObjects();
            DrawText(gameWin ? "WIN" : "LOSE", SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2 - 50, 40, GOLD);
            DrawText("Press space to restart", SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT / 2 - 10, 20, GOLD);
            EndDrawing();
        }

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
        DrawText(TextFormat("SIDEWAYS V: %f", sidewaysVelocity), 3, 105, 20, GRAY);
        DrawText(TextFormat("TIME TILL TUMBLE: %f", (lastTumbleTime + tumbleInterval) - GetTime() ), 3, 126, 20, GRAY);
#endif
        // UI TEXT
        DrawText(TextFormat("DRUNKNESS : %s", ((drunkTier == SOBER) ? "SOBER" : ((drunkTier == TIPSY) ? "TIPSY" : "NAUSEOUS"))),
                            3, 0, 20,
                            (drunkTier == SOBER) ? GREEN : (drunkTier == TIPSY) ? YELLOW : RED);        
        
        if (drunkTier == NAUSEOUS) {
            DrawText(TextFormat("GET TO EVIL CORP OR GET SOBER BEFORE YOU PUKE"), (SCREEN_WIDTH / 2) - 400, SCREEN_HEIGHT - 100, 30, DARKPURPLE);
            DrawText(TextFormat("Puke Countdown : %i", static_cast<int>(nauseousCountdown)), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 50, 20, DARKPURPLE);
        }
    
        EndDrawing();
    }

    GraphicsCleanup();
    GameLogicCleanup();
    CloseWindow();

    return 0;
}
