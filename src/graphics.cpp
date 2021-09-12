#include "graphics.h"
#include "game_logic.h"
#include "models.h"
#include "raylib.h"
#include "raymath.h"
#include "collisions.h"
static constexpr float camera_back = 5.0f;
static constexpr float camera_height = 2.0f;
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
    DrawModelEx(charModel, { playerPos.x ,playerPos.y + .4f,playerPos.z }, { .0f,1.0f,.0f }, playerRot, { 1.0f,1.0f,1.0f }, WHITE);
    DrawModelEx(playerModel, playerPos, { .0f,1.0f,.0f }, 90 + playerRot, { 1.0f,1.0f,1.0f }, WHITE);
    //Draw Collectables
    for (int i = 0;i < collectables.size(); i++) {
        if (collectables[i].active) {
            DrawModel(beerModel, { collectables[i].location.x,.2f,collectables[i].location.y }, 3.0f, WHITE);
        }
    }
    for (int i = 0; i < numPuddles; i++){
        PuddleState puddle = puddles[i];
        DrawCircle3D({ puddle.posX, 0.0, puddle.posY }, puddle.size, { 1,0,0 }, 90.0, ORANGE);
    }
    

    //Draw Obstacles
    for (int i = 0;i < obstacles.size();i++) {
        if (obstacles[i].type == SHORT_WALL_COLLIDER)
            DrawModelEx(smallWallModel, { obstacles[i].pos.x,0.5f,obstacles[i].pos.y }, { .0f,1.0f,.0f }, obstacles[i].rot, { 1.0f,1.0f,1.0f }, WHITE);
        else if (obstacles[i].type == BIG_WALL_COLLIDER)
            DrawModelEx(bigWallModel, { obstacles[i].pos.x,0.5f,obstacles[i].pos.y }, { .0f,1.0f,.0f }, obstacles[i].rot, { 1.0f,1.0f,1.0f }, WHITE);
    }
    DrawModel(floorModel, { .0f,-.1f,.0f }, 1.0f, DARKGRAY);
    //Draw objective
    DrawModel(bspModel, { -40.0f,.0f,.0f }, .3f, BROWN);

    //DrawGrid(200, 1.0f);
    EndMode3D();
}
void GraphicsCleanup() {
    ModelsCleanup();
}