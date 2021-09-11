#include "game_logic.h"
#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
DrawnState drawnState;
constexpr float acceleration = .1f;
constexpr float turnSpeed = 1.0f;//in deg/s 
constexpr float normalMaxSpeed = 10.0f;
float maxSpeed = normalMaxSpeed;

// puddle params
PuddleState* puddles;
double slide_time = -10.0;
double slideDuration = 1.0;
float post_slide_deceleration = 5.0f;
int num_puddles = 1;
bool in_a_puddle = false;
bool slidable = false;

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
void GameLogicInit(){ 
    // init puddles
    puddles = static_cast<PuddleState*>(malloc(num_puddles*sizeof(PuddleState))); 
    
    PuddleState puddle0 = {0.0f, 30.0f, 5.0f};
    puddles[0] = puddle0;
}
void GameLogicUpdate() {
    float timeDelta = GetFrameTime();

    #pragma region Slide Player
    // set collision states for all puddles
    bool in_a_puddle_prev_frame = in_a_puddle;
    in_a_puddle = false;
    for (int i = 0; i < num_puddles; i++)
    {
        Vector2 playerPos = {drawnState.playerX, drawnState.playerY};
        Vector2 puddlePos = {puddles[i].posX, puddles[i].posY};
        bool in_this_puddle = Vector2Length(Vector2Subtract(playerPos, puddlePos)) < puddles[i].size;
        if (in_this_puddle)
        {
            in_a_puddle = true; 
        }
    }

    // make player slide if we have just entered a puddle
    if (in_a_puddle && !in_a_puddle_prev_frame){
        maxSpeed *= 2.0f;
        velocity *= 2.0f;
        slide_time = GetTime();
    }
    if (GetTime() - slide_time > slideDuration){
        maxSpeed = normalMaxSpeed - post_slide_deceleration * timeDelta; // decelerate
        maxSpeed = maxSpeed > normalMaxSpeed ? maxSpeed : normalMaxSpeed; // clamp decelaration
    }
    #pragma endregion

    HandleInput();
    drawnState.playerX += cos(drawnState.playerRot * DEG2RAD) * velocity * timeDelta;
    // subtract due to GL coord system (Z towards for our camera)
    drawnState.playerY -= sin(drawnState.playerRot * DEG2RAD) * velocity * timeDelta;

    
}
void GameLogicCleanup(){
    free(puddles);
}