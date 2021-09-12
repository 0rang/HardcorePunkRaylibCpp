#include "game_logic.h"
#include "collisions.h"
#include "raylib.h"
#include "raymath.h"
#include "collisions.h"
#include <stdlib.h>
constexpr float acceleration = .1f;
constexpr float turnSpeed = 1.0f;//in deg/s 
constexpr float defaultMaxSpeed = 10.0f;
float maxSpeed = defaultMaxSpeed;
Vector3 playerPos = { .0f,.0f,.0f };
float playerRot = 90.0f;
constexpr float reboundFade = 2.0f;
static Vector2 collisionRebound = {};
bool gameLogicLocked = false;


#pragma region Puddle Params
constexpr double slideDuration = 1.0;
constexpr float postSlideDeceleration = 5.0f;
const int numPuddles = 1;
PuddleState* puddles;
double slideTime = -10.0;
bool inAPuddle = false;
#pragma endregion

#pragma region Jump Params
constexpr float initialJumpVelocity = 5.6f;
constexpr float gravity = 9.0f; //should be positive
bool grounded = true;
float jumpVelocity = 0.0f;
#pragma endregion

#pragma region Drunk Movement Params
constexpr double nauseousTimeLimit = 8.0f; // starts when nauseous; about to throw up; need to sober up or get to the goal 
constexpr double defaultTumbleInterval = 3.0f;
constexpr float tumbleSpeed = 5.0f;
constexpr float tumbleDrag = 4.0f;
double nauseousStartTime;
double nauseousCountdown;
bool nauseousTimerStarted = false;
double tumbleInterval = defaultTumbleInterval; // configure based on drunkness and make slight random adjustments each time
double lastTumbleTime = -10.0;
DrunkTier drunkTier = SOBER;
float sidewaysVelocity = 0.0f;
float swayDir; // direction of tumble (1.0 or -1.0)
#pragma endregion 

#pragma region Corrupt Corp Params
float corpPosX = 5;
float corpPosZ = -5;
float corpTriggerDist;
#pragma endregion



float velocity = .0f; //in m/s
void HandleInput() {
    if (IsKeyDown(KEY_W) && grounded)
        velocity += acceleration;
    if (IsKeyDown(KEY_S) && grounded)
        velocity -= 2 * acceleration;
    if (IsKeyDown(KEY_A))
        playerRot += turnSpeed;
    if (IsKeyDown(KEY_D))
        playerRot -= turnSpeed;
}
void GameLogicInit() {
    // allocate puddles
    puddles = static_cast<PuddleState*>(malloc(numPuddles * sizeof(PuddleState)));
    PuddleState puddle0 = { 0.0f, -30.0f, 2.0f };
    puddles[0] = puddle0;
}

// TODO: actually pause the game here and tell the player what happened
void LockGameLogic(){
    gameLogicLocked = true;
    velocity = 0;
    sidewaysVelocity = 0;
}

void ResetGameState() {
    drunkTier = SOBER;
    playerPos = { 0.0f, 0.0f, 0.0f };
    for (Collectable& collec : collectables) {
        collec.active = true;
    }

    // re-randomize collectables
    collectables.clear();
    CollectablesInit();
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

        // make player slide if we have just entered a puddle and player is grounded
        if (inAPuddle && !in_a_puddle_prev_frame && grounded) {
            maxSpeed *= 2.0f;
            velocity *= 2.0f;
            slideTime = GetTime();
        }
        if (GetTime() - slideTime > slideDuration) {
            maxSpeed = maxSpeed - postSlideDeceleration * timeDelta; // decelerate
            maxSpeed = maxSpeed > defaultMaxSpeed ? maxSpeed : defaultMaxSpeed; // clamp decelaration
        }
    }
#pragma endregion

#pragma region Jump
    if (IsKeyPressed(KEY_SPACE) && grounded) {
        jumpVelocity = initialJumpVelocity;
    }
    playerPos.y += jumpVelocity * timeDelta;
    jumpVelocity -= gravity * timeDelta;
    playerPos.y = playerPos.y > 0.0f ? playerPos.y : 0.0f; // don't fall through the ground
    grounded = playerPos.y <= 0.0f;
#pragma endregion

#pragma region Set Drunkness
    for (int i = 0; i < collectables.size(); i++) {
        if (collectables[i].active && collectables[i].hits({ playerPos.x, playerPos.z })) {
            collectables[i].active = false;
            drunkTier = static_cast<DrunkTier>(drunkTier + 1);
            lastTumbleTime = GetTime();
            if (drunkTier == BLACKOUT)
            {
                LockGameLogic();
            }
            
        }
    }
#pragma endregion

    // make player tumble when drunk and start game end timer when nauseated
#pragma region Drunk Effects
    if (velocity > maxSpeed / 2.0f && drunkTier > SOBER &&
        GetTime() > lastTumbleTime + tumbleInterval) {
        swayDir = (rand() % 2) ? 1.0f : -1.0f; // randomly picks left or right tumble
        sidewaysVelocity = tumbleSpeed * swayDir;
        lastTumbleTime = GetTime();
    }

    sidewaysVelocity -= tumbleDrag * swayDir * timeDelta; // dampen tumble velocity
    // if swayDir is positive, clamp to 0 when sidewaysVelocity goes negative and vice versa
    if (swayDir > 0) {
        sidewaysVelocity = (sidewaysVelocity > 0.0f) ? sidewaysVelocity : 0.0f;
    }
    else {
        sidewaysVelocity = (sidewaysVelocity < 0.0f) ? sidewaysVelocity : 0.0f;
    }

    playerPos.x += cos((playerRot + 90.0f) * DEG2RAD) * sidewaysVelocity * timeDelta;
    playerPos.z -= sin((playerRot + 90.0f) * DEG2RAD) * sidewaysVelocity * timeDelta;

    // set end timer 
    if (drunkTier == NAUSEOUS) {
        if (!nauseousTimerStarted) {
            nauseousStartTime = GetTime();
            nauseousTimerStarted = true;
        }
        else if (GetTime() > nauseousStartTime + nauseousTimeLimit) {
            LockGameLogic();
        }

    }
    else {
        nauseousTimerStarted = false;
    }
    
    nauseousCountdown = nauseousStartTime + nauseousTimeLimit - GetTime();
    

#pragma endregion

#pragma region Corrupt Corp Logic
    {
        Vector2 corpPos = {corpPosX, corpPosZ};

        // if (Vector2Length(corp))
        // {
        //     /* code */
        // }
        
    }
#pragma endregion

    // clamp velocity
    velocity = (velocity < maxSpeed) * velocity + (velocity >= maxSpeed) * maxSpeed;
    velocity = (velocity > 0.0f) * velocity;

    playerPos.x += cos(playerRot * DEG2RAD) * velocity * timeDelta;
    // subtract due to GL coord system (Z towards for our camera)
    playerPos.z -= sin(playerRot * DEG2RAD) * velocity * timeDelta;

    playerCollider.pos = { playerPos.x,playerPos.z };
    playerCollider.rot = playerRot + 90;
    //handle collisions
    for (int i = 0;i < obstacles.size();i++) {
        if (CheckColliders(playerCollider, obstacles[i]) && (obstacles[i].type == BIG_WALL_COLLIDER || playerPos.y < 1.0f)) {
            Vector2 pushback = FindPushBack(playerCollider, obstacles[i]);
            playerPos.x -= pushback.x;
            playerPos.z += pushback.y;
            if (velocity > 5.0f) {
                velocity /= 2.0f;
                drunkTier = static_cast<DrunkTier>((drunkTier > SOBER) ? (drunkTier - 1) : drunkTier);
            }
        }
    }

}
void GameLogicCleanup() {
    free(puddles);
}