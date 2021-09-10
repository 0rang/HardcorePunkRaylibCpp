#include "graphics.h"
#include "game_logic.h"
#include "raylib.h"
#include "raymath.h"
Camera mainCamera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };
void GraphicsInit() {}
void Draw3DObjects() {
    //Camera follows player
    mainCamera.position.x = drawnState.playerX;
    mainCamera.position.z = 10 + drawnState.playerY;
    mainCamera.target.x = drawnState.playerX;
    mainCamera.target.z = drawnState.playerY;
    BeginMode3D(mainCamera);

    //Draw Player
    //position
    DrawCube({ drawnState.playerX, 0.0f, drawnState.playerY }, 1.0f, 1.0f, 1.0f, BLUE);
    //look direction
    DrawLine3D({ drawnState.playerX, 0.0f,drawnState.playerY }, { drawnState.playerX + 2 * cos(DEG2RAD *
               drawnState.playerRot),.0f,drawnState.playerY - 2 * sin(DEG2RAD * drawnState.playerRot) }, BLACK);
    DrawGrid(100, 1.0f);
    EndMode3D();
}
void GraphicsCleanup() {}