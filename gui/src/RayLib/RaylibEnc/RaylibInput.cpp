/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibEncInput
*/

#include "RayLibEnc.hpp"
#include "../../Utils/GamepadConstants.hpp"

bool RayLibEnc::isMouseButtonDown(int button) const
{
    if (button == MOUSE_BUTTON_LEFT && isGamepadButtonDown(0, GAMEPAD_BUTTON_A))
        return true;

    return IsMouseButtonDown(button);
}

bool RayLibEnc::isMouseButtonPressed(int button) const
{
    if (button == MOUSE_BUTTON_LEFT && isGamepadButtonPressed(0, GAMEPAD_BUTTON_A))
        return true;

    return IsMouseButtonPressed(button);
}

bool RayLibEnc::isMouseButtonReleased(int button) const
{
    if (button == MOUSE_BUTTON_LEFT && IsGamepadButtonReleased(0, GAMEPAD_BUTTON_A))
        return true;

    return IsMouseButtonReleased(button);
}

bool RayLibEnc::isKeyDown(int key) const
{
    return IsKeyDown(key);
}

bool RayLibEnc::isKeyPressed(int key) const
{
    return IsKeyPressed(key);
}

bool RayLibEnc::isKeyReleased(int key) const
{
    return IsKeyReleased(key);
}

Vector2 RayLibEnc::getMousePosition() const
{
    return GetMousePosition();
}

void RayLibEnc::setMousePosition(int x, int y)
{
    SetMousePosition(x, y);
}

void RayLibEnc::disableCursor()
{
    if (!_isCursorLocked) {
        DisableCursor();
        _isCursorLocked = true;
    }
}

void RayLibEnc::enableCursor()
{
    if (_isCursorLocked) {
        EnableCursor();
        _isCursorLocked = false;
    }
}

int RayLibEnc::getScreenWidth() const
{
    return GetScreenWidth();
}

int RayLibEnc::getScreenHeight() const
{
    return GetScreenHeight();
}

Vector2 RayLibEnc::getMouseDelta()
{
    if (_isCursorLocked) {
        return GetMouseDelta();
    } else {
        Vector2 currentMousePosition = GetMousePosition();
        Vector2 delta = { currentMousePosition.x - _previousMousePosition.x,
            currentMousePosition.y - _previousMousePosition.y };

        this->_previousMousePosition = currentMousePosition;
        return delta;
    }
}

float RayLibEnc::getMouseWheelMove() const
{
    return GetMouseWheelMove();
}
