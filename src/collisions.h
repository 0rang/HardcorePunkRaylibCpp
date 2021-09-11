#pragma once
#include "raylib.h"
#include <cstdlib>
#include <vector>
struct ObjectCollider {
    BoundingBox bbox;
    Vector2 pos = { 0.0f,0.0f };
    float rot = 0.0f;
    ObjectCollider() {};
    ObjectCollider(const Model& model);
};
extern ObjectCollider playerCollider;
extern std::vector<ObjectCollider> obstacles;
void ObstaclesInit();
bool CheckColliders(const ObjectCollider& o1, const ObjectCollider& o2);