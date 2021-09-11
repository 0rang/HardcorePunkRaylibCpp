#pragma once
#include "raylib.h"
#include <cstdlib>
#include <vector>
struct ObjectCollider {
    Vector2* verts = NULL;
    Vector2 pos = { 0.0f,0.0f };
    unsigned int count = 0;
    float rot = 0.0f;
    ObjectCollider() {};
    ObjectCollider(const Model& model);
    ~ObjectCollider() {
        if (verts != NULL)
            free(verts);
    }
};
extern ObjectCollider playerCollider;
extern std::vector<ObjectCollider> obstacles;
void ObstaclesInit();
bool CheckColliders(const ObjectCollider& o1, const ObjectCollider& o2);