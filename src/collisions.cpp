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
    bbox = MeshBoundingBox(model.meshes[0]);
}
float ProjectAxis(Vector2 axis, Vector2 point) {
    float x = ((point.x * axis.x + point.y * axis.y) * axis.x / (axis.x * axis.x + axis.y * axis.y));
    float y = ((point.x * axis.x + point.y * axis.y) * axis.y / (axis.x * axis.x + axis.y * axis.y));
    return Vector2DotProduct({ x,y }, axis);
}
bool CheckColliders(const ObjectCollider& o1, const ObjectCollider& o2) {
    //tranform o2 to o1 local space
    Vector2 newpos = Vector2Subtract(o2.pos, o1.pos);
    float newrot = DEG2RAD * (o2.rot - o1.rot);
    Vector2 corners[4];
    corners[0] = { static_cast<float>((o2.bbox.max.x + newpos.x) * cos(newrot) + (o2.bbox.max.z + newpos.y) * sin(newrot)) ,
    static_cast<float>((o2.bbox.max.x + newpos.x) * sin(newrot) - (o2.bbox.max.z + newpos.y) * cos(newrot)) };
    corners[1] = { static_cast<float>((o2.bbox.max.x + newpos.x) * cos(newrot) + (o2.bbox.min.z + newpos.y) * sin(newrot)) ,
    static_cast<float>((o2.bbox.max.x + newpos.x) * sin(newrot) - (o2.bbox.min.z + newpos.y) * cos(newrot)) };
    corners[2] = { static_cast<float>((o2.bbox.min.x + newpos.x) * cos(newrot) + (o2.bbox.min.z + newpos.y) * sin(newrot)) ,
    static_cast<float>((o2.bbox.min.x + newpos.x) * sin(newrot) - (o2.bbox.min.z + newpos.y) * cos(newrot)) };
    corners[3] = { static_cast<float>((o2.bbox.min.x + newpos.x) * cos(newrot) + (o2.bbox.max.z + newpos.y) * sin(newrot)) ,
    static_cast<float>((o2.bbox.min.x + newpos.x) * sin(newrot) - (o2.bbox.max.z + newpos.y) * cos(newrot)) };
    float maxX, minX, maxY, minY;
    // project o2 to unit x
    maxX = corners[0].x;
    minX = corners[0].x;
    for (int i = 1;i < 4;i++) {
        if (corners[i].x > maxX)
            maxX = corners[i].x;
        if (corners[i].x < minX)
            minX = corners[i].x;
    }
    if (o1.bbox.min.x > maxX || minX > o1.bbox.max.x)
        return false;
    //project o2 to unit y
    maxY = corners[0].y;
    minY = corners[0].y;
    for (int i = 1;i < 4;i++) {
        if (corners[i].y > maxY)
            maxY = corners[i].y;
        if (corners[i].y < minY)
            minY = corners[i].y;
    }
    if (o1.bbox.min.z > maxY || minY > o1.bbox.max.z)
        return false;

    //project o1 to first axis of o2
    Vector2 axis = Vector2Subtract(corners[0], corners[1]);
    maxX = ProjectAxis(axis, { o1.bbox.max.x,o1.bbox.max.z });
    minX = maxX;
    float res = ProjectAxis(axis, { o1.bbox.max.x,o1.bbox.min.z });
    maxX = res > maxX ? res : maxX;
    minX = res < minX ? res : minX;
    res = ProjectAxis(axis, { o1.bbox.min.x,o1.bbox.min.z });
    maxX = res > maxX ? res : maxX;
    minX = res < minX ? res : minX;
    res = ProjectAxis(axis, { o1.bbox.min.x,o1.bbox.max.z });
    maxX = res > maxX ? res : maxX;
    minX = res < minX ? res : minX;
    minY = ProjectAxis(axis, corners[0]);
    maxY = minY;
    for (int i = 1;i < 4;i++) {
        res = ProjectAxis(axis, corners[i]);
        maxY = res > maxY ? res : maxY;
        minY = res < minY ? res : minY;
    }
    if (minX > maxY || minY > maxX)
        return false;

    //project o1 to second axis of o2
    axis = Vector2Subtract(corners[1], corners[2]);
    maxX = ProjectAxis(axis, { o1.bbox.max.x,o1.bbox.max.z });
    minX = maxX;
    res = ProjectAxis(axis, { o1.bbox.max.x,o1.bbox.min.z });
    maxX = res > maxX ? res : maxX;
    minX = res < minX ? res : minX;
    res = ProjectAxis(axis, { o1.bbox.min.x,o1.bbox.min.z });
    maxX = res > maxX ? res : maxX;
    minX = res < minX ? res : minX;
    res = ProjectAxis(axis, { o1.bbox.min.x,o1.bbox.max.z });
    maxX = res > maxX ? res : maxX;
    minX = res < minX ? res : minX;
    minY = ProjectAxis(axis, corners[0]);
    maxY = minY;
    for (int i = 1;i < 4;i++) {
        res = ProjectAxis(axis, corners[i]);
        maxY = res > maxY ? res : maxY;
        minY = res < minY ? res : minY;
    }
    if (minX > maxY || minY > maxX)
        return false;

    return true;
}