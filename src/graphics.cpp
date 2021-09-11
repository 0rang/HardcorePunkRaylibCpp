#include "graphics.h"
#include "game_logic.h"
#include "models.h"
#include "raylib.h"
#include "raymath.h"
static constexpr float camera_back = 5.0f;
static constexpr float camera_height = 4.0f;
Camera mainCamera = { { 0.0f, camera_height, camera_back }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };
void GraphicsInit() {
    ModelsInit();
}
void Draw3DObjects() {
    //Camera follows player
#ifdef PLAYER_CAMERA_FIXED
    mainCamera.position.x = drawnState.playerX;
    mainCamera.position.z = 10 + drawnState.playerY;
#else
    mainCamera.position.x = -camera_back * cos(DEG2RAD * drawnState.playerRot) + drawnState.playerX;
    mainCamera.position.z = camera_back * sin(DEG2RAD * drawnState.playerRot) + drawnState.playerY;
#endif
    mainCamera.target.x = drawnState.playerX;
    mainCamera.target.z = drawnState.playerY;
    BeginMode3D(mainCamera);

    //Draw Player
    //position
    //hardcoded scale
    DrawModelEx(playerModel, { drawnState.playerX,0.0f,drawnState.playerY }, { .0f,1.0f,.0f }, 90 + drawnState.playerRot, { 1.0f,1.0f,1.0f }, WHITE);
    //look direction
    /*
    DrawLine3D({ drawnState.playerX, 0.0f, drawnState.playerY }, {
        drawnState.playerX + static_cast <float>(2 * cos(DEG2RAD *
            drawnState.playerRot)), .0f, drawnState.playerY - static_cast <float>(2 * sin(DEG2RAD * drawnState.playerRot))
        }, BLACK);
    */
    DrawGrid(100, 1.0f);
    EndMode3D();
}
void GraphicsCleanup() {
    ModelsCleanup();
}