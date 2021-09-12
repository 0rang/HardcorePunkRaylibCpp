#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <vector>
#include "raylib.h"

struct Collectable {
    Vector2 location;
    bool active = true;
    bool hits(const Vector2& playerPos);
};
void CollectablesInit();
extern std::vector<Collectable> collectables;

struct PuddleState {
    float posX = .0f;
    float posY = .0f;
    float size = 2.0f;
};
extern PuddleState* puddles;
extern const int numPuddles;

enum DrunkTier {
    SOBER,
    TIPSY,
    NAUSEOUS,
    BLACKOUT
};
extern DrunkTier drunkTier;
extern float sidewaysVelocity;
extern double lastTumbleTime;
extern double tumbleInterval;

extern double nauseousCountdown;

extern bool gameLogicLocked;

extern Vector3 playerPos;
extern float playerRot;

extern float velocity;
extern bool grounded;
void GameLogicInit();
void GameLogicUpdate();
void ResetGameState();
void GameLogicCleanup();
#endif