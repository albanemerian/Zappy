/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibEncGamepad
*/

#include <iostream>
#include "RayLibEnc.hpp"
#include "../../Utils/GamepadConstants.hpp"

bool RayLibEnc::isGamepadAvailable(int gamepad) const
{
    return IsGamepadAvailable(gamepad);
}

bool RayLibEnc::isGamepadButtonPressed(int gamepad, int button) const
{
    if (!isGamepadAvailable(gamepad))
        return false;

    return IsGamepadButtonPressed(gamepad, button);
}

bool RayLibEnc::isGamepadButtonDown(int gamepad, int button) const
{
    if (!isGamepadAvailable(gamepad))
        return false;

    return IsGamepadButtonDown(gamepad, button);
}

bool RayLibEnc::isGamepadButtonReleased(int gamepad, int button) const
{
    if (!isGamepadAvailable(gamepad))
        return false;

    return IsGamepadButtonReleased(gamepad, button);
}

float RayLibEnc::getGamepadAxisMovement(int gamepad, int axis) const
{
    if (!isGamepadAvailable(gamepad))
        return 0.0f;

    return GetGamepadAxisMovement(gamepad, axis);
}
