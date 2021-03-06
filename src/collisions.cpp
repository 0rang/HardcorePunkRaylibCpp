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
    playerCollider.type = PLAYER_COLLIDER;
    playerCollider.bbox.min.x *= 0.19992423057556152f;
    playerCollider.bbox.max.x *= 0.19992423057556152f;
    ObjectCollider o2(bigWallModel);
    o2.pos.y = -10.0f;
    o2.pos.x = -1.0f;
    o2.type = BIG_WALL_COLLIDER;
    o2.bbox.min.z *= .125;
    o2.bbox.max.z *= .125;
    for (float y = -100;y < 90;y += 4.5f) {
        for (float x = -30;x < 30;x += 1) {
            if (rand() % 2) {
                if (rand() % 2) {
                    o.pos.x = x;
                    o.pos.y = y;
                    obstacles.push_back(o);
                }
                else {
                    o2.pos.x = x;
                    o2.pos.y = y;
                    obstacles.push_back(o2);
                }
            }
        }
    }


}
ObjectCollider::ObjectCollider(const Model& model) {
    bbox = MeshBoundingBox(model.meshes[0]);
}
Vector2 ProjectAxisXY(Vector2 axis, Vector2 point) {
    float x = ((point.x * axis.x + point.y * axis.y) * axis.x / (axis.x * axis.x + axis.y * axis.y));
    float y = ((point.x * axis.x + point.y * axis.y) * axis.y / (axis.x * axis.x + axis.y * axis.y));
    return { x,y };
}
float ProjectAxis(Vector2 axis, Vector2 point) {
    return Vector2DotProduct(ProjectAxisXY(axis, point), axis);
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
Vector2 FindPushBack(ObjectCollider target, ObjectCollider obstacle) {
    Vector2 obstacleNormals[4];
    obstacleNormals[0] = { static_cast<float>(abs(obstacle.bbox.max.x) * cos(DEG2RAD * obstacle.rot)),
    static_cast<float>(-(abs(obstacle.bbox.max.x)) * sin(DEG2RAD * obstacle.rot)) };
    obstacleNormals[1] = { static_cast<float>(abs(obstacle.bbox.max.z) * cos(DEG2RAD * (obstacle.rot - 90))),
    static_cast<float>(-(abs(obstacle.bbox.max.z)) * sin(DEG2RAD * (obstacle.rot - 90))) };
    obstacleNormals[2] = { static_cast<float>(abs(obstacle.bbox.min.x) * cos(DEG2RAD * (obstacle.rot - 180))),
    static_cast<float>(-(abs(obstacle.bbox.min.x)) * sin(DEG2RAD * (obstacle.rot - 180))) };
    obstacleNormals[3] = { static_cast<float>(abs(obstacle.bbox.min.z) * cos(DEG2RAD * (obstacle.rot - 270))),
    static_cast<float>(-(abs(obstacle.bbox.min.z)) * sin(DEG2RAD * (obstacle.rot - 270))) };
    //find closest corner
    Vector2 targetOffset = Vector2Subtract(target.pos, obstacle.pos);
    Vector2 closestCorner = { target.bbox.max.x + targetOffset.x,target.bbox.max.z + targetOffset.y };
    float closestDist = (target.bbox.max.x + targetOffset.x) * (target.bbox.max.x + targetOffset.x)
        + (target.bbox.max.z + targetOffset.y) * (target.bbox.max.z + targetOffset.y);
    float res = (target.bbox.max.x + targetOffset.x) * (target.bbox.max.x + targetOffset.x)
        + (target.bbox.min.z + targetOffset.y) * (target.bbox.min.z + targetOffset.y);
    if (res < closestDist) {
        closestDist = res;
        closestCorner = { target.bbox.max.x + targetOffset.x,target.bbox.min.z + targetOffset.y };
    }
    res = (target.bbox.min.x + targetOffset.x) * (target.bbox.min.x + targetOffset.x)
        + (target.bbox.min.z + targetOffset.y) * (target.bbox.min.z + targetOffset.y);
    if (res < closestDist) {
        closestDist = res;
        closestCorner = { target.bbox.min.x + targetOffset.x,target.bbox.min.z + targetOffset.y };
    }
    res = (target.bbox.min.x + targetOffset.x) * (target.bbox.min.x + targetOffset.x)
        + (target.bbox.max.z + targetOffset.y) * (target.bbox.max.z + targetOffset.y);
    if (res < closestDist) {
        closestDist = res;
        closestCorner = { target.bbox.min.x + targetOffset.x,target.bbox.max.z + targetOffset.y };
    }
    Vector2 projectedDiff = Vector2Subtract(obstacleNormals[0], ProjectAxisXY(obstacleNormals[0], closestCorner));
    for (int i = 1;i < 4;i++) {
        Vector2 candidateDiff = Vector2Subtract(obstacleNormals[i], ProjectAxisXY(obstacleNormals[i], closestCorner));
        if ((candidateDiff.x) * (candidateDiff.x) + (candidateDiff.y) * (candidateDiff.y) < (projectedDiff.x) * (projectedDiff.x) + (projectedDiff.y) * (projectedDiff.y))
            projectedDiff = candidateDiff;
    }
    return projectedDiff;
}