/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibEncCamera
*/

#include "RayLibEnc.hpp"
#include "raymath.h"
#include "../../Utils/Constants.hpp"
#include "../../Utils/GamepadConstants.hpp"

void RayLibEnc::initCamera()
{
    _camera.position = (Vector3){ -10.0f, 10.0f, -10.0f };
    _camera.target = (Vector3){ 10.0f, -5.0f, 10.0f };
    _camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    _camera.fovy = 45.0f;
    _camera.projection = CAMERA_PERSPECTIVE;
}

void RayLibEnc::setCameraPosition(Vector3 position)
{
    _camera.position = position;
}

void RayLibEnc::setCameraTarget(Vector3 target)
{
    _camera.target = target;
}

void RayLibEnc::setCameraUp(Vector3 up)
{
    _camera.up = up;
}

void RayLibEnc::setCameraFovy(float fovy)
{
    _camera.fovy = fovy;
}

void RayLibEnc::setCameraProjection(int projection)
{
    _camera.projection = projection;
}

void RayLibEnc::updateCamera(int mode)
{
    UpdateCamera(&_camera, mode);
}

void RayLibEnc::updateCameraFreeMode(float camMovingSpeed, float camRotaSpeed)
{
    updateLastInputType();

    float deltaTime = getFrameTime();
    float moveSpeed = camMovingSpeed * deltaTime;
    float rotationSpeed = zappy::gui::CAMERA_SENSITIVITY;

    Vector3 forward = Vector3Subtract(_camera.target, _camera.position);
    forward = Vector3Normalize(forward);

    Vector3 right = Vector3CrossProduct(forward, _camera.up);
    right = Vector3Normalize(right);

    if (IsKeyDown(KEY_Z) || IsKeyDown(KEY_W)) {
        _camera.position = Vector3Add(_camera.position,
                                    Vector3Scale(forward, moveSpeed));
        _camera.target = Vector3Add(_camera.target, Vector3Scale(forward, moveSpeed));
    }
    if (IsKeyDown(KEY_S)) {
        _camera.position = Vector3Subtract(_camera.position,
                                        Vector3Scale(forward, moveSpeed));
        _camera.target = Vector3Subtract(_camera.target, Vector3Scale(forward, moveSpeed));
    }
    if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_A)) {
        _camera.position = Vector3Subtract(_camera.position,
                                        Vector3Scale(right, moveSpeed));
        _camera.target = Vector3Subtract(_camera.target, Vector3Scale(right, moveSpeed));
    }
    if (IsKeyDown(KEY_D)) {
        _camera.position = Vector3Add(_camera.position, Vector3Scale(right, moveSpeed));
        _camera.target = Vector3Add(_camera.target, Vector3Scale(right, moveSpeed));
    }
    if (IsKeyDown(KEY_SPACE) || isGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER)) {
        _camera.position.y += moveSpeed * 0.5;
        _camera.target.y += moveSpeed * 0.5;
    }
    if (IsKeyDown(KEY_LEFT_CONTROL) || isGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER)) {
        _camera.position.y -= moveSpeed * 0.5;
        _camera.target.y -= moveSpeed * 0.5;
    }

    if (isMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        int screenCenterX = getScreenWidth() / 2;
        int screenCenterY = getScreenHeight() / 2;

        setMousePosition(screenCenterX, screenCenterY);
        _previousMousePosition = Vector2{ static_cast<float>(screenCenterX),
            static_cast<float>(screenCenterY) };
        disableCursor();
    }

    if (isMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        enableCursor();

    if (isMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouseDelta = getMouseDelta();

        if (mouseDelta.x != 0) {
            Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
            float rotAngle = mouseDelta.x * rotationSpeed;

            float cosRotAngle = cosf(rotAngle);
            float sinRotAngle = sinf(rotAngle);
            float newDirX = viewDir.x * cosRotAngle - viewDir.z * sinRotAngle;
            float newDirZ = viewDir.x * sinRotAngle + viewDir.z * cosRotAngle;
            viewDir.x = newDirX;
            viewDir.z = newDirZ;

            _camera.target = Vector3Add(_camera.position, viewDir);
        }

        if (mouseDelta.y != 0) {
            Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
            float rotAngle = -mouseDelta.y * rotationSpeed;

            Vector3 rightVec = Vector3CrossProduct(viewDir, _camera.up);
            rightVec = Vector3Normalize(rightVec);
            viewDir = Vector3RotateByAxisAngle(viewDir, rightVec, rotAngle);

            Vector3 newUp = Vector3CrossProduct(rightVec, viewDir);
            if (newUp.y > 0.0f)
                _camera.target = Vector3Add(_camera.position, viewDir);
        }

        if (_isCursorLocked) {
            int screenCenterX = getScreenWidth() / 2;
            int screenCenterY = getScreenHeight() / 2;
            setMousePosition(screenCenterX, screenCenterY);
        }
    }

    if (IsKeyDown(KEY_RIGHT)) {
        Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
        float rotAngle = camRotaSpeed * deltaTime;

        float cosRotAngle = cosf(rotAngle);
        float sinRotAngle = sinf(rotAngle);
        float newDirX = viewDir.x * cosRotAngle - viewDir.z * sinRotAngle;
        float newDirZ = viewDir.x * sinRotAngle + viewDir.z * cosRotAngle;
        viewDir.x = newDirX;
        viewDir.z = newDirZ;

        _camera.target = Vector3Add(_camera.position, viewDir);
    }

    if (IsKeyDown(KEY_LEFT)) {
        Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
        float rotAngle = -camRotaSpeed * deltaTime;

        float cosRotAngle = cosf(rotAngle);
        float sinRotAngle = sinf(rotAngle);
        float newDirX = viewDir.x * cosRotAngle - viewDir.z * sinRotAngle;
        float newDirZ = viewDir.x * sinRotAngle + viewDir.z * cosRotAngle;
        viewDir.x = newDirX;
        viewDir.z = newDirZ;

        _camera.target = Vector3Add(_camera.position, viewDir);
    }

    if (IsKeyDown(KEY_UP)) {
        Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
        float rotAngle = camRotaSpeed * deltaTime;

        Vector3 rightVec = Vector3CrossProduct(viewDir, _camera.up);
        rightVec = Vector3Normalize(rightVec);
        viewDir = Vector3RotateByAxisAngle(viewDir, rightVec, rotAngle);

        Vector3 newUp = Vector3CrossProduct(rightVec, viewDir);
        if (newUp.y > 0.0f)
            _camera.target = Vector3Add(_camera.position, viewDir);
    }

    if (IsKeyDown(KEY_DOWN)) {
        Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
        float rotAngle = -camRotaSpeed * deltaTime;

        Vector3 rightVec = Vector3CrossProduct(viewDir, _camera.up);
        rightVec = Vector3Normalize(rightVec);
        viewDir = Vector3RotateByAxisAngle(viewDir, rightVec, rotAngle);

        Vector3 newUp = Vector3CrossProduct(rightVec, viewDir);
        if (newUp.y > 0.0f)
            _camera.target = Vector3Add(_camera.position, viewDir);
    }

    if (isGamepadAvailable(0)) {
        float rightStickX = getGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
        float rightStickY = getGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

        if (fabs(rightStickX) > zappy::gui::GAMEPAD_DEADZONE) {
            Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
            float rotAngle = rightStickX * zappy::gui::GAMEPAD_STICK_SENSITIVITY * deltaTime;

            float cosRotAngle = cosf(rotAngle);
            float sinRotAngle = sinf(rotAngle);
            float newDirX = viewDir.x * cosRotAngle - viewDir.z * sinRotAngle;
            float newDirZ = viewDir.x * sinRotAngle + viewDir.z * cosRotAngle;
            viewDir.x = newDirX;
            viewDir.z = newDirZ;

            _camera.target = Vector3Add(_camera.position, viewDir);
        }

        if (fabs(rightStickY) > zappy::gui::GAMEPAD_DEADZONE) {
            Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
            float rotAngle = -rightStickY * zappy::gui::GAMEPAD_STICK_SENSITIVITY * deltaTime;

            Vector3 rightVec = Vector3CrossProduct(viewDir, _camera.up);
            rightVec = Vector3Normalize(rightVec);
            viewDir = Vector3RotateByAxisAngle(viewDir, rightVec, rotAngle);

            Vector3 newUp = Vector3CrossProduct(rightVec, viewDir);
            if (newUp.y > 0.0f)
                _camera.target = Vector3Add(_camera.position, viewDir);
        }

        float leftStickX = getGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float leftStickY = getGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

        Vector3 tforward = Vector3Subtract(_camera.target, _camera.position);
        tforward = Vector3Normalize(tforward);

        Vector3 tright = Vector3CrossProduct(tforward, _camera.up);
        tright = Vector3Normalize(tright);

        if (fabs(leftStickY) > zappy::gui::GAMEPAD_DEADZONE) {
            float moveAmount = -leftStickY * moveSpeed;
            _camera.position =
                Vector3Add(_camera.position, Vector3Scale(tforward, moveAmount));
            _camera.target = Vector3Add(_camera.target, Vector3Scale(tforward, moveAmount));
        }

        if (fabs(leftStickX) > zappy::gui::GAMEPAD_DEADZONE) {
            float moveAmount = leftStickX * moveSpeed;
            _camera.position = Vector3Add(_camera.position, Vector3Scale(tright, moveAmount));
            _camera.target = Vector3Add(_camera.target, Vector3Scale(tright, moveAmount));
        }
    }
}

Camera3D RayLibEnc::getCamera() const
{
    return _camera;
}
