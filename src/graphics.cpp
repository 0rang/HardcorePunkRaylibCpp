#include "graphics.h"
#include "game_logic.h"
#include "models.h"
#include "raylib.h"
#include "raymath.h"
#include "collisions.h"
static constexpr float camera_back = 5.0f;
static constexpr float camera_height = 4.0f;
Camera mainCamera = { { 0.0f, camera_height, camera_back }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };
void GraphicsInit() {
    ModelsInit();
}
void Draw3DObjects() {
    //Camera follows player
#ifdef PLAYER_CAMERA_FIXED
    mainCamera.position.x = playerPos.x;
    mainCamera.position.z = 10 + playerPos.z;
#else
    mainCamera.position.x = -camera_back * cos(DEG2RAD * playerRot) + playerPos.x;
    mainCamera.position.y = camera_height + playerPos.y;
    mainCamera.position.z = camera_back * sin(DEG2RAD * playerRot) + playerPos.z;
#endif
    mainCamera.target = playerPos;
    BeginMode3D(mainCamera);

    //Draw Player
    //position
    DrawModelEx(playerModel, playerPos, { .0f,1.0f,.0f }, 90 + playerRot, { 1.0f,1.0f,1.0f }, WHITE);
    //Draw Collectables
    for (int i = 0;i < collectables.size(); i++) {
        if (collectables[i].active) {
            if (collectables[i].hits({ playerPos.x,playerPos.z }))
                DrawModel(beerModel, { collectables[i].location.x,.1f,collectables[i].location.y }, 1.0f, GREEN);
            else
                DrawModel(beerModel, { collectables[i].location.x,.1f,collectables[i].location.y }, 1.0f, WHITE);
        }
    }
    DrawCircle3D({ puddles[0].posX, 0.0, puddles[0].posY }, puddles[0].size, { 1,0,0 }, 90.0, ORANGE);
    playerCollider.pos = { playerPos.x,playerPos.z };
    playerCollider.rot = playerRot + 90;
    //Draw Obstacles
    for (int i = 0;i < obstacles.size();i++) {
        if (CheckColliders(playerCollider, obstacles[i]))
            DrawModelEx(smallWallModel, { obstacles[i].pos.x,0.5f,obstacles[i].pos.y }, { .0f,1.0f,.0f }, obstacles[i].rot, { 1.0f,1.0f,1.0f }, RED);
        else
            DrawModelEx(smallWallModel, { obstacles[i].pos.x,0.5f,obstacles[i].pos.y }, { .0f,1.0f,.0f }, obstacles[i].rot, { 1.0f,1.0f,1.0f }, WHITE);
    }
    DrawModel(floorModel, { .0f,-.1f,.0f }, 1.0f, DARKGRAY);
    DrawGrid(200, 1.0f);
    EndMode3D();
}
void GraphicsCleanup() {
    ModelsCleanup();
}