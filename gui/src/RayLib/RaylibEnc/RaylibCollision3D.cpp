/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibCollision3D
*/

#include "RayLibEnc.hpp"
#include "raymath.h"

Ray RayLibEnc::getMouseRay(Vector2 mousePosition)
{
    return GetScreenToWorldRay(mousePosition, _camera);
}

RayCollision RayLibEnc::getRayCollisionBox(Ray ray, BoundingBox box)
{
    return GetRayCollisionBox(ray, box);
}

RayCollision RayLibEnc::getRayCollisionSphere(Ray ray, Vector3 center, float radius)
{
    return GetRayCollisionSphere(ray, center, radius);
}

bool RayLibEnc::checkCollisionBoxes(BoundingBox box1, BoundingBox box2)
{
    return CheckCollisionBoxes(box1, box2);
}

Ray RayLibEnc::getMouseRayFromCurrent()
{
    Vector2 mousePos = getMousePosition();
    return getMouseRay(mousePos);
}

BoundingBox RayLibEnc::createBoundingBox(Vector3 center, Vector3 size)
{
    Vector3 halfSize = {size.x / 2.0f, size.y / 2.0f, size.z / 2.0f};
    BoundingBox box;
    box.min = Vector3Subtract(center, halfSize);
    box.max = Vector3Add(center, halfSize);
    return box;
}

BoundingBox RayLibEnc::createBoundingBoxFromMinMax(Vector3 min, Vector3 max)
{
    BoundingBox box;
    box.min = min;
    box.max = max;
    return box;
}
