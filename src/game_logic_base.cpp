#include "game_logic.h"
#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
constexpr float acceleration = .1f;
constexpr float turnSpeed = 1.0f;//in deg/s 
constexpr float normalMaxSpeed = 10.0f;
float maxSpeed = normalMaxSpeed;
Vector3 playerPos = { .0f,.0f,.0f };
float playerRot = 90.0f;

#pragma region Puddle Params
constexpr double slideDuration = 1.0;
constexpr float postSlideDeceleration = 5.0f;
constexpr int numPuddles = 1;
PuddleState* puddles;
double slideTime = -10.0;
bool inAPuddle = false;
#pragma endregion

#pragma region Jump Params
constexpr float maxJumpHeight = 2.0f;
constexpr float initialJumpVelocity = 15.0f;
constexpr float gravity = 14.0f; //should be positive
bool jumping = false;
float jumpVelocity = 0.0f;
#pragma endregion

float velocity = .0f; //in m/s
void HandleInput() {
    if (IsKeyDown(KEY_W))
        velocity += acceleration;
    if (IsKeyDown(KEY_S))
        velocity -= 2 * acceleration;
    if (IsKeyDown(KEY_A))
        playerRot += turnSpeed;
    if (IsKeyDown(KEY_D))
        playerRot -= turnSpeed;
}
void GameLogicInit() {
    // init puddles
    puddles = static_cast<PuddleState*>(malloc(numPuddles * sizeof(PuddleState)));

    PuddleState puddle0 = { 0.0f, -30.0f, 2.0f };
    puddles[0] = puddle0;
}
void GameLogicUpdate() {
    float timeDelta = GetFrameTime();
    HandleInput();

#pragma region Slide in Puddle
    {
    bool in_a_puddle_prev_frame = inAPuddle;
    inAPuddle = false;

    // check all puddle distances from player
    for (int i = 0; i < numPuddles; i++) {
        Vector2 playerPos2d = { playerPos.x, playerPos.z };
        Vector2 puddlePos = { puddles[i].posX, puddles[i].posY };
        // is distance to player pos smaller than puddle radius?
        if (Vector2Length(Vector2Subtract(playerPos2d, puddlePos)) < puddles[i].size) {
            inAPuddle = true;
        }
    }

    // make player slide if we have just entered a puddle
    if (inAPuddle && !in_a_puddle_prev_frame) {
        maxSpeed *= 2.0f;
        velocity *= 2.0f;
        slideTime = GetTime();
    }
    if (GetTime() - slideTime > slideDuration) {
        maxSpeed = maxSpeed - postSlideDeceleration * timeDelta; // decelerate
        maxSpeed = maxSpeed > normalMaxSpeed ? maxSpeed : normalMaxSpeed; // clamp decelaration
    }
    }
#pragma endregion
    
#pragma region Jump
    if(IsKeyPressed(KEY_SPACE) && playerPos.y <= 0.0f){
        jumpVelocity = initialJumpVelocity;
    }
    playerPos.y += jumpVelocity * timeDelta;
    jumpVelocity -= gravity * timeDelta;
    playerPos.y = playerPos.y > 0.0f ? playerPos.y : 0.0f; // don't fall through the ground
#pragma endregion

    // clamp velocity
    velocity = (velocity < maxSpeed) * velocity + (velocity >= maxSpeed) * maxSpeed;
    velocity = (velocity > 0.0f) * velocity;

    playerPos.x += cos(playerRot * DEG2RAD) * velocity * timeDelta;
    // subtract due to GL coord system (Z towards for our camera)
    playerPos.z -= sin(playerRot * DEG2RAD) * velocity * timeDelta;
}
void GameLogicCleanup() {
    free(puddles);
}