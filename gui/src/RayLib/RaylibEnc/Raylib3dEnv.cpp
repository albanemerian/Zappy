/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibEnc3dEnv
*/

#include "RayLibEnc.hpp"
#include "raymath.h"

void RayLibEnc::begin3DMode()
{
    BeginMode3D(_camera);
}

void RayLibEnc::end3DMode()
{
    EndMode3D();
}

float RayLibEnc::vector3Distance(Vector3 v1, Vector3 v2) const
{
    return Vector3Distance(v1, v2);
}

Vector3 RayLibEnc::vector3Normalize(Vector3 v) const
{
    return Vector3Normalize(v);
}

Vector3 RayLibEnc::vector3Subtract(Vector3 v1, Vector3 v2) const
{
    return Vector3Subtract(v1, v2);
}

Vector3 RayLibEnc::vector3Add(Vector3 v1, Vector3 v2) const
{
    return Vector3Add(v1, v2);
}
