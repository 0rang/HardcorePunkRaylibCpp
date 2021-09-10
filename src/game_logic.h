#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
// parts of game logic that influence drawing
// don't know if this is a good idea
// TODO: probably refactor
struct DrawnState {
    float playerX = .0f;
    float playerY = .0f;
    float playerRot = 90.0f;
};
extern DrawnState drawnState;
extern float velocity;
void GameLogicUpdate();
#endif