#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <vector>
#include "raylib.h"
// parts of game logic that influence drawing
// don't know if this is a good idea
// TODO: probably refactor
struct DrawnState {
    float playerX = .0f;
    float playerY = .0f;
    float playerRot = 90.0f;
};
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
    float size = .5f;

    bool player_in_puddle = false;
};

extern PuddleState* puddles;
extern DrawnState drawnState;
extern float velocity;
void GameLogicInit();
void GameLogicUpdate();
void GameLogicCleanup();
#endif