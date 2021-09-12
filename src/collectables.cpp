#include "game_logic.h"
#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
std::vector<Collectable> collectables;
constexpr unsigned int collectableCount = 30;
void CollectablesInit() {
    std::srand(std::time(NULL));
    Collectable c;
    for (int i = 0;i < collectableCount;i++) {
        c.location.x = 50.0f - rand() % 1000 / 10.0f;
        c.location.y = 50.0f - rand() % 1000 / 10.0f;
        collectables.push_back(c);
    }
}
bool Collectable::hits(const Vector2& playerPos) {
    //Check distance via Pythagorean theorem
    return((playerPos.x - location.x) * (playerPos.x - location.x) +
        (playerPos.y - location.y) * (playerPos.y - location.y) <= .5f);
}