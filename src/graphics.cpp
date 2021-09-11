#include "graphics.h"
#include "game_logic.h"
#include "raylib.h"
#include "raymath.h"
Camera mainCamera = { { 0.0f, 8.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };
void GraphicsInit() {}
void Draw3DObjects() {
    //Camera follows player
#ifdef PLAYER_CAMERA_FIXED
    mainCamera.position.x = drawnState.playerX;
    mainCamera.position.z = 10 + drawnState.playerY;
#else
    mainCamera.position.x = -10 * cos(DEG2RAD * drawnState.playerRot) + drawnState.playerX;
    mainCamera.position.z = 10 * sin(DEG2RAD * drawnState.playerRot) + drawnState.playerY;
#endif
    mainCamera.target.x = drawnState.playerX;
    mainCamera.target.z = drawnState.playerY;
    BeginMode3D(mainCamera);

    //Draw Player
    //position
    DrawCube({ drawnState.playerX, 0.0f, drawnState.playerY }, 1.0f, 1.0f, 1.0f, BLUE);
    //look direction
    DrawLine3D({ drawnState.playerX, 0.0f, drawnState.playerY }, {
        drawnState.playerX + static_cast <float>(2 * cos(DEG2RAD *
            drawnState.playerRot)), .0f, drawnState.playerY - static_cast <float>(2 * sin(DEG2RAD * drawnState.playerRot))
        }, BLACK);
    DrawGrid(100, 1.0f);
    EndMode3D();
}
void GraphicsCleanup() {}