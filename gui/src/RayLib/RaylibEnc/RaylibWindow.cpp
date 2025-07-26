/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** WindowMethods
*/

#include <string>
#include "RayLibEnc.hpp"

void RayLibEnc::initWindow(int width, int height, const std::string &title)
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(width, height, title.c_str());
    SetExitKey(0);
    _isInitialized = true;
}

void RayLibEnc::closeWindow()
{
    if (_isInitialized) {
        CloseWindow();
        _isInitialized = false;
    }
}

bool RayLibEnc::windowShouldClose() const
{
    return WindowShouldClose();
}

void RayLibEnc::beginDrawing()
{
    BeginDrawing();
}

void RayLibEnc::endDrawing()
{
    EndDrawing();
}

void RayLibEnc::clearBackground(Color color)
{
    ClearBackground(color);
}

bool RayLibEnc::isWindowReady() const
{
    return IsWindowReady();
}

int RayLibEnc::getMonitorWidth(int monitor) const
{
    return GetMonitorWidth(monitor);
}

int RayLibEnc::getMonitorHeight(int monitor) const
{
    return GetMonitorHeight(monitor);
}

void RayLibEnc::waitTime(float seconds) const
{
    WaitTime(seconds);
}

void RayLibEnc::setTargetFPS(int fps) const
{
    SetTargetFPS(fps);
}

int RayLibEnc::getFPS() const
{
    return GetFPS();
}

float RayLibEnc::getFrameTime() const
{
    return GetFrameTime();
}
