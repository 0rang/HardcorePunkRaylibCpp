#pragma once
#include "raylib.h"
#include <cstdlib>
#include <vector>
enum ColliderType {
    PLAYER_COLLIDER,
    SHORT_WALL_COLLIDER,
    BIG_WALL_COLLIDER
};
struct ObjectCollider {
    ColliderType type = SHORT_WALL_COLLIDER;
    BoundingBox bbox;
    Vector2 pos = { 0.0f,0.0f };
    float rot = 0.0f;
    ObjectCollider() {};
    ObjectCollider(const Model& model);
};
extern ObjectCollider playerCollider;
extern std::vector<ObjectCollider> obstacles;
void ObstaclesInit();
Vector2 FindPushBack(ObjectCollider target, ObjectCollider obstruction);
bool CheckColliders(const ObjectCollider& o1, const ObjectCollider& o2);