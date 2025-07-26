/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibSkybox
*/

#include <iostream>
#include <cmath>
#include <string>
#include "RayLibEnc.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "../../Utils/Constants.hpp"

bool RayLibEnc::loadSkybox(const std::string& id, const std::string& filepath)
{
    Mesh mesh = GenMeshSphere(1.0f, 64, 32);

    for (int i = 0; i < mesh.vertexCount * 3; i++) {
        mesh.normals[i] = -mesh.normals[i];
    }

    for (int i = 0; i < mesh.vertexCount; i++) {
        float x = mesh.vertices[3*i];
        float y = mesh.vertices[3*i + 1];
        float z = mesh.vertices[3*i + 2];

        float theta = atan2f(y, x);
        float phi = asinf(z);

        float u = (theta + PI) / (2.0f * PI);
        float v = (phi + PI/2.0f) / PI;

        mesh.texcoords[2*i] = u;
        mesh.texcoords[2*i + 1] = 1.0f - v;
    }

    Model skyboxSphere = LoadModelFromMesh(mesh);
    Texture texture = LoadTexture(filepath.c_str());

    if (texture.id == 0) {
        std::cout << "Warning: Could not load skybox texture, using default" << std::endl;
        Image img = GenImageGradientRadial(512, 512, 0.0f, SKYBLUE, BLUE);
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    skyboxSphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
    skyboxSphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;

    ModelData skyboxData;
    skyboxData.model = skyboxSphere;
    skyboxData.animationCount = 0;
    skyboxData.center = {0.0f, 0.0f, 0.0f};

    _models[id] = skyboxData;
    _textures[id] = texture;
    return true;
}

Color RayLibEnc::getDayNightColor(float cycleTime)
{
    if (cycleTime < 0.25f) {
        float t = cycleTime / 0.25f;
        return {
            (unsigned char)(20 + t * 30),   // R: 20 -> 50
            (unsigned char)(30 + t * 60),   // G: 30 -> 90
            (unsigned char)(80 + t * 60),   // B: 80 -> 140
            255
        };
    } else if (cycleTime < 0.4f) {
        float t = (cycleTime - 0.25f) / 0.15f;
        return {
            (unsigned char)(50 + t * 190),  // R: 50 -> 240
            (unsigned char)(90 + t * 140),  // G: 90 -> 230
            (unsigned char)(140 + t * 80),  // B: 140 -> 220
            255
        };
    } else if (cycleTime < 0.65f) {
        float t = (cycleTime - 0.4f) / 0.25f;
        return {
            (unsigned char)(240 + t * 15),  // R: 240 -> 255
            (unsigned char)(230 + t * 25),  // G: 230 -> 255
            (unsigned char)(220 + t * 35),  // B: 220 -> 255
            255
        };
    } else if (cycleTime < 0.8f) {
        float t = (cycleTime - 0.65f) / 0.15f;
        return {
            (unsigned char)(255),            // R: 255
            (unsigned char)(255 - t * 155),  // G: 255 -> 100
            (unsigned char)(255 - t * 155),  // B: 255 -> 100
            255
        };
    } else {
        float t = (cycleTime - 0.8f) / 0.2f;
        return {
            (unsigned char)(255 - t * 235),  // R: 255 -> 20
            (unsigned char)(100 - t * 70),   // G: 100 -> 30
            (unsigned char)(100 - t * 20),   // B: 100 -> 80
            255
        };
    }
}

void RayLibEnc::drawSkybox(const std::string& id)
{
    auto it = _models.find(id);
    if (it == _models.end()) {
        std::cout << "Skybox " << id << " not found" << std::endl;
        return;
    }

    Vector3 position = _camera.position;

    float cycleTime = fmodf(GetTime() / zappy::gui::DURATION_DAYNIGHT_CYCLE + 0.5f, 1.0f);
    Color dayNightTint = getDayNightColor(cycleTime);

    float timeRotation = GetTime() * 2.5f;
    rlDisableBackfaceCulling();

    Matrix rotationX = MatrixRotateX(90.0f * DEG2RAD);
    Matrix rotationY = MatrixRotateY(timeRotation * DEG2RAD);
    Matrix finalRotation = MatrixMultiply(rotationX, rotationY);

    Vector3 axis;
    float angle;
    QuaternionToAxisAngle(QuaternionFromMatrix(finalRotation), &axis, &angle);

    DrawModelEx(it->second.model, position,
        axis, angle * RAD2DEG, {500.0f, 500.0f, 500.0f}, dayNightTint);
    rlEnableBackfaceCulling();
}
