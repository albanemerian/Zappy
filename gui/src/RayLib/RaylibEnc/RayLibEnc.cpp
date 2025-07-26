/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RayLibEnc
*/

#define NDEBUG
#define SUPPORT_TRACELOG_DEBUG    0
#define TRACELOG_SILENT

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "../../Utils/Constants.hpp"
#include "../../Utils/GamepadConstants.hpp"
#include "RayLibEnc.hpp"

RayLibEnc::RayLibEnc() : _isInitialized(false), _isCursorLocked(false),
    _lastInputType(InputType::NONE)
{
    _camera = {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        0.0f, CAMERA_PERSPECTIVE };
    _previousMousePosition = (Vector2){ 0.0f, 0.0f };
}

RayLibEnc::~RayLibEnc()
{
    if (_isInitialized)
        closeWindow();

    unloadAllModels();
    unloadAllFonts();
}

void RayLibEnc::drawRectangleRec(Rectangle rec, Color color)
{
    DrawRectangleRec(rec, color);
}

void RayLibEnc::drawText(const std::string& text, float x, float y, float fontSize,
    Color color)
{
    if (_fonts.find("default") != _fonts.end()) {
        float scaledFontSize = getScaledFontSize(fontSize);
        float spacing = getFontSpacing(scaledFontSize);

        rlPushMatrix();
        rlTranslatef(x, y, 0.0f);
        rlScalef(FONT_RENDER_SCALE, FONT_RENDER_SCALE, 1.0f);

        DrawTextEx(_fonts["default"], text.c_str(),
                   {0.0f, 0.0f},
                   scaledFontSize, spacing, color);

        rlPopMatrix();
    } else {
        DrawText(text.c_str(),
                 static_cast<int>(x),
                 static_cast<int>(y),
                 static_cast<int>(fontSize),
                 color);
    }
}

void RayLibEnc::drawTextEx(const std::string& text, float x, float y, float fontSize,
    float spacing, Color color)
{
    if (_fonts.find("default") != _fonts.end()) {
        float scaledFontSize = getScaledFontSize(fontSize);
        float scaledSpacing = getScaledSpacing(spacing);

        rlPushMatrix();
        rlTranslatef(x, y, 0.0f);
        rlScalef(FONT_RENDER_SCALE, FONT_RENDER_SCALE, 1.0f);

        DrawTextEx(_fonts["default"], text.c_str(),
                   {0.0f, 0.0f},
                   scaledFontSize, scaledSpacing, color);

        rlPopMatrix();
    } else {
        DrawText(text.c_str(),
                 static_cast<int>(x),
                 static_cast<int>(y),
                 static_cast<int>(fontSize),
                 color);
    }
}

void RayLibEnc::drawCircle(float centerX, float centerY, float radius, Color color)
{
    DrawCircle(static_cast<int>(centerX), static_cast<int>(centerY), radius, color);
}

void RayLibEnc::drawCircleLines(float centerX, float centerY, float radius, Color color)
{
    DrawCircleLines(static_cast<int>(centerX), static_cast<int>(centerY), radius, color);
}

float RayLibEnc::measureText(const std::string& text, float fontSize) const
{
    if (_fonts.find("default") != _fonts.end()) {
        float scaledFontSize = getScaledFontSize(fontSize);
        float spacing = getFontSpacing(scaledFontSize);
        Vector2 textSize = MeasureTextEx(_fonts.at("default"), text.c_str(),
            scaledFontSize, spacing);
        return textSize.x * FONT_RENDER_SCALE;
    }
    return static_cast<float>(MeasureText(text.c_str(), static_cast<int>(fontSize)));
}

float RayLibEnc::measureTextEx(const std::string& text, float fontSize, float spacing) const
{
    if (_fonts.find("default") != _fonts.end()) {
        float scaledFontSize = getScaledFontSize(fontSize);
        float scaledSpacing = getScaledSpacing(spacing);
        Vector2 textSize = MeasureTextEx(_fonts.at("default"), text.c_str(),
            scaledFontSize, scaledSpacing);
        return textSize.x * FONT_RENDER_SCALE;
    }
    return static_cast<float>(MeasureText(text.c_str(), static_cast<int>(fontSize)));
}

void RayLibEnc::drawTextureRec(Texture2D texture, Rectangle source, Vector2 position,
    Color tint)
{
    DrawTextureRec(texture, source, position, tint);
}

void RayLibEnc::unloadTexture(Texture2D texture)
{
    UnloadTexture(texture);
}

bool RayLibEnc::checkCollisionPointRec(Vector2 point, Rectangle rec) const
{
    return CheckCollisionPointRec(point, rec);
}

void RayLibEnc::beginScissorMode(int x, int y, int width, int height)
{
    BeginScissorMode(x, y, width, height);
}

void RayLibEnc::endScissorMode()
{
    EndScissorMode();
}

bool RayLibEnc::loadFont(const std::string& id, const std::string& filepath)
{
    int fontBaseSize = 96;
    Font font = LoadFontEx(filepath.c_str(), fontBaseSize, 0, 250);

    if (font.texture.id == 0) {
        std::cout << "Failed to load font: " << filepath << std::endl;

        font = LoadFont(filepath.c_str());
        if (font.texture.id == 0)
            return false;
    }

    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    _fonts[id] = font;
    return true;
}

void RayLibEnc::unloadFont(const std::string& id)
{
    auto it = _fonts.find(id);
    if (it != _fonts.end()) {
        UnloadFont(it->second);
        _fonts.erase(it);
    }
}

bool RayLibEnc::hasFontLoaded(const std::string& id) const
{
    return _fonts.find(id) != _fonts.end();
}

Font RayLibEnc::getFont(const std::string& id) const
{
    auto it = _fonts.find(id);
    if (it != _fonts.end()) {
        return it->second;
    }
    return GetFontDefault();
}

void RayLibEnc::unloadAllFonts()
{
    for (auto& fontPair : _fonts) {
        UnloadFont(fontPair.second);
    }
    _fonts.clear();
}

InputType RayLibEnc::getLastInputType() const
{
    return _lastInputType;
}

void RayLibEnc::updateLastInputType()
{
    if (isGamepadAvailable(0)) {
        for (int button = GAMEPAD_BUTTON_LEFT_FACE_UP; button <= GAMEPAD_BUTTON_RIGHT_THUMB;
                ++button) {
            if (isGamepadButtonPressed(0, button) || isGamepadButtonDown(0, button)) {
                _lastInputType = InputType::GAMEPAD;
                return;
            }
        }

        for (int axis = GAMEPAD_AXIS_LEFT_X; axis <= GAMEPAD_AXIS_RIGHT_Y; ++axis) {
            float axisValue = getGamepadAxisMovement(0, axis);
            if (fabs(axisValue) > 0.1f) {
                _lastInputType = InputType::GAMEPAD;
                return;
            }
        }
    }

    for (int key = KEY_SPACE; key <= KEY_KB_MENU; ++key) {
        if (isKeyPressed(key) || isKeyDown(key)) {
            _lastInputType = InputType::KEYBOARD_MOUSE;
            return;
        }
    }

    for (int button = MOUSE_BUTTON_LEFT; button <= MOUSE_BUTTON_EXTRA; ++button) {
        if (isMouseButtonPressed(button) || isMouseButtonDown(button)) {
            _lastInputType = InputType::KEYBOARD_MOUSE;
            return;
        }
    }

    Vector2 mouseDelta = getMouseDelta();
    if (fabs(mouseDelta.x) > 0.1f || fabs(mouseDelta.y) > 0.1f) {
        _lastInputType = InputType::KEYBOARD_MOUSE;
        return;
    }

    if (fabs(getMouseWheelMove()) > 0.01f) {
        _lastInputType = InputType::KEYBOARD_MOUSE;
        return;
    }
}

float RayLibEnc::getTime() const
{
    return GetTime();
}

float RayLibEnc::getScaledFontSize(float fontSize) const
{
    return fontSize * FONT_SCALE_FACTOR;
}

float RayLibEnc::getFontSpacing(float scaledFontSize) const
{
    return scaledFontSize * FONT_SPACING_RATIO;
}

float RayLibEnc::getScaledSpacing(float spacing) const
{
    return spacing * FONT_SCALE_FACTOR;
}
