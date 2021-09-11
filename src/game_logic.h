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
extern DrawnState drawnState;
extern float velocity;
void GameLogicUpdate();
#endif