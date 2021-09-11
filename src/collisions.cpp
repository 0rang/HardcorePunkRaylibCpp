#include "collisions.h"
#include "raylib.h"
#include "raymath.h"
#include "models.h"
#include <cstdlib>
#include <vector>
#include <cstdio>
std::vector<ObjectCollider> obstacles;
ObjectCollider playerCollider;
void ObstaclesInit() {
    ObjectCollider o(smallWallModel);
    playerCollider = ObjectCollider(playerModel);
    o.pos.y = 10.0f;
    obstacles.push_back(o);
}
ObjectCollider::ObjectCollider(const Model& model) {
    BoundingBox bbox = MeshBoundingBox(model.meshes[0]);
    count = 4;
    verts = (Vector2*)malloc(count * sizeof(Vector2));
    verts[0] = { bbox.max.x,bbox.max.z };
    verts[1] = { bbox.max.x,bbox.min.z };
    verts[2] = { bbox.min.x,bbox.min.z };
    verts[3] = { bbox.min.x,bbox.max.z };
}
bool LinesInterSect(Vector2& a1, Vector2 a2, Vector2 b1, Vector2 b2) {
    bool aVert = false, bVert = false;
    float v1, v2, u1, u2;
    if (a1.x == a2.x)
        aVert = true;
    else {
        u1 = (a1.y - a2.y) / (a1.x - a2.x);
        v1 = a1.y - u1 * a1.x;
    }
    if (b1.x == b2.x)
        bVert = true;
    else {
        u1 = (b1.y - b2.y) / (b1.x - b2.x);
        v1 = b1.y - u1 * b1.x;
    }

}
bool CheckColliders(const ObjectCollider& o1, const ObjectCollider& o2) {
    int intersections = 0;
    for (int i = 0;i < o1.count;i++) {
        for (int j = 0;j < o2.count;j++) {
            Vector2 p1 = { o2.pos.x + o2.verts[j].x * cos(o2.rot),o2.pos.y - o2.verts[j].y * sin(o2.rot) },
                p2 = { o2.pos.x + o2.verts[(j + 1) % o2.count].x * cos(o2.rot),
                o2.pos.y - o2.verts[(j + 1) % o2.count].y * sin(o2.rot) };
            if (p1.x == p2.x && (o1.verts[i].y >= p1.y && o1.verts[i].y <= p2.y) || (o1.verts[i].y < p1.y && o1.verts[i].y >= p2.y)) {
                intersections++;
                continue;
            }
            float a = (p1.y - p2.y) / (p1.x - p2.x);
            float b = p1.y - a * p1.x;
            float x3 = (o1.verts[i].y - b) / a;
            if ((x3 >= p1.x && x3 <= p2.x) || (x3 < p1.x && x3 >= p2.x))
                intersections++;
        }
        if (intersections % 2)
            return true;
    }
    return false;
}