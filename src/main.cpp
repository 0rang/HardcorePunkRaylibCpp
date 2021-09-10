#include "raylib.h"
#include "graphics.h"
#include "game_logic.h"
#include "models.h"

constexpr auto SCREEN_WIDTH = 1600;
constexpr auto SCREEN_HEIGHT = 900;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window title");
    SetTargetFPS(60);
    GraphicsInit();

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png");

    while (!WindowShouldClose()) {
        GameLogicUpdate();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Draw3DObjects();
        DrawText(TextFormat("X: %f", drawnState.playerX), 3, 0, 20, BLACK);
        DrawText(TextFormat("Y: %f", drawnState.playerY), 3, 21, 20, BLACK);
        DrawText(TextFormat("V: %f", velocity), 3, 42, 20, BLACK);
        EndDrawing();
    }

    GraphicsCleanup();
    CloseWindow();

    return 0;
}
