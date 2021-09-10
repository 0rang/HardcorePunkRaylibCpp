#include "game_logic.h"
#include "raylib.h"
#include "raymath.h"
DrawnState drawnState;
constexpr float acceleration = .1f;
constexpr float turnSpeed = 1.0f;//in deg/s 
constexpr float maxSpeed = 10.0f;


float velocity = .0f; //in m/s
void HandleInput() {
    if (IsKeyDown(KEY_W)) {
        velocity += acceleration;
        velocity = (velocity < maxSpeed) * velocity + (velocity >= maxSpeed) * maxSpeed;
    }
    if (IsKeyDown(KEY_S)) {
        velocity -= 2 * acceleration;
        velocity = (velocity > 0.0f) * velocity;
    }
    if (IsKeyDown(KEY_A))
        drawnState.playerRot += turnSpeed;
    if (IsKeyDown(KEY_D))
        drawnState.playerRot -= turnSpeed;
}
void GameLogicUpdate() {
    float timeDelta = GetFrameTime();
    HandleInput();
    drawnState.playerX += cos(drawnState.playerRot * DEG2RAD) * velocity * timeDelta;
    // subtract due to GL coord system (Z towards for our camera)
    drawnState.playerY -= sin(drawnState.playerRot * DEG2RAD) * velocity * timeDelta;
}