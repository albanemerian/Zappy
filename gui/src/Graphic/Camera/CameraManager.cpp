/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CameraManager
*/

#include <cmath>
#include <algorithm>
#include "CameraManager.hpp"
#include "raymath.h"
#include "../../Utils/GamepadConstants.hpp"

CameraManager::CameraManager(std::shared_ptr<IDisplay> display) : _display(display),
    _mapCenter({ 0.0f, 0.0f, 0.0f }), _mapWidth(0), _mapHeight(0),
    _targetDistance(30.0f), _targetAngleXZ(0.0f), _targetAngleY(0.5f), _isDragging(false),
    _playerId(-1), _playerAngleXZ(0.75f), _isPlayerViewDragging(false)
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::setTargetDistance(float distance)
{
    if (distance < 5.0f)
        distance = 5.0f;

    if (distance > 100.0f)
        distance = 100.0f;

    _targetDistance = distance;
}

void CameraManager::initTargetPositionFromCurrentCamera()
{
    float distance = this->_display->vector3DDistanceFromCamera(_mapCenter);
    Vector3f direction = this->_display->vector3SubtractFromCamera(_mapCenter);

    direction = this->_display->vector3Normalize(direction);
    _targetAngleY = asinf(direction.y);
    _targetAngleXZ = atan2f(direction.z, direction.x);
    _targetDistance = distance;
}

void CameraManager::updateCamera(zappy::gui::CameraMode mode)
{
    switch (mode) {
        case zappy::gui::CameraMode::FREE:
            updateCameraFreeMode();
            break;
        case zappy::gui::CameraMode::TARGETED:
            updateCameraTargetMode();
            break;
        case zappy::gui::CameraMode::PLAYER:
            updateCameraPlayerMode();
            break;
        default:
            updateCameraFreeMode();
            break;
    }
}

void CameraManager::updateCameraFreeMode()
{
    this->_display->updateLastInputType();
    this->_display->updateCameraFreeMode(this->_cameraMovingSpeed, this->_cameraRotaSpeed);
}

void CameraManager::updateCameraTargetMode()
{
    this->_display->updateLastInputType();
    float deltaTime = this->_display->getFrameTime();

    if (this->_display->isMouseButtonPressed(this->_display->getKeyId(MOUSE_RIGHT))) {
        _isDragging = true;

        Vector2i screenSize = this->_display->getScreenSize();
        int screenCenterX = screenSize.x / 2;
        int screenCenterY = screenSize.y / 2;

        this->_display->setMousePosition({static_cast<float>(screenCenterX),
            static_cast<float>(screenCenterY)});
        this->_display->disableCursor();
    }

    if (this->_display->isMouseButtonReleased(this->_display->getKeyId(MOUSE_RIGHT))) {
        _isDragging = false;
        this->_display->enableCursor();
    }

    if (_isDragging) {
        Vector2f mouseDelta = this->_display->getMouseDelta();
        const float rotationSensitivity = zappy::gui::CAMERA_SENSITIVITY;

        _targetAngleXZ += mouseDelta.x * rotationSensitivity;
        _targetAngleY -= mouseDelta.y * rotationSensitivity;

        const float maxVerticalAngle = 1.5f;
        if (_targetAngleY > maxVerticalAngle) _targetAngleY = maxVerticalAngle;
        if (_targetAngleY < 0.1f) _targetAngleY = 0.1f;

        if (_isDragging) {
            Vector2i screenSize = this->_display->getScreenSize();
            int screenCenterX = screenSize.x / 2;
            int screenCenterY = screenSize.y / 2;
            this->_display->setMousePosition({static_cast<float>(screenCenterX),
                static_cast<float>(screenCenterY)});
        }
    }

    float wheelMove = this->_display->getMouseWheelMove();
    if (wheelMove != 0) {
        float zoomSpeed = this->_cameraZoomSpeed * deltaTime;
        _targetDistance -= wheelMove * zoomSpeed;

        if (_targetDistance < 5.0f) _targetDistance = 5.0f;
        if (_targetDistance > 100.0f) _targetDistance = 100.0f;
    }

    bool isZoomingIn = this->_display->isGamepadButtonDown(
        this->_display->getKeyId(GM_PD_RIGHT_TRIGGER));
    bool isZoomingOut = this->_display->isGamepadButtonDown(
        this->_display->getKeyId(GM_PD_LEFT_TRIGGER));
    if (isZoomingIn) {
        _targetDistance -= this->_cameraMovingSpeed * deltaTime;
        if (_targetDistance < 5.0f) _targetDistance = 5.0f;
    }
    if (isZoomingOut) {
        _targetDistance += this->_cameraMovingSpeed * deltaTime;
        if (_targetDistance > 100.0f) _targetDistance = 100.0f;
    }

    if (this->_display->isKeyDown(this->_display->getKeyId(RIGHT)))
        _targetAngleXZ += this->_cameraRotaSpeed * deltaTime;

    if (this->_display->isKeyDown(this->_display->getKeyId(LEFT)))
        _targetAngleXZ -= this->_cameraRotaSpeed * deltaTime;

    if (this->_display->isGamepadAvailable()) {
        float rightStickX = this->_display->getGamepadAxisMovement(
            this->_display->getKeyId(GM_PD_AXIS_RIGHT_X));
        float rightStickY = this->_display->getGamepadAxisMovement(
            this->_display->getKeyId(GM_PD_AXIS_RIGHT_Y));

        if (fabs(rightStickX) > zappy::gui::GAMEPAD_DEADZONE)
            _targetAngleXZ += rightStickX * zappy::gui::GAMEPAD_STICK_SENSITIVITY * deltaTime;

        if (fabs(rightStickY) > zappy::gui::GAMEPAD_DEADZONE) {
            _targetAngleY -= rightStickY * zappy::gui::GAMEPAD_STICK_SENSITIVITY * deltaTime;

            const float maxVerticalAngle = 1.5f;
            if (_targetAngleY > maxVerticalAngle) _targetAngleY = maxVerticalAngle;
            if (_targetAngleY < 0.1f) _targetAngleY = 0.1f;
        }
    }

    if (this->_display->isKeyDown(this->_display->getKeyId(UP))) {
        _targetAngleY += this->_cameraRotaSpeed * deltaTime;
        if (_targetAngleY > 1.5f) _targetAngleY = 1.5f;
    }

    if (this->_display->isKeyDown(this->_display->getKeyId(DOWN))) {
        _targetAngleY -= this->_cameraRotaSpeed * deltaTime;
        if (_targetAngleY < 0.1f) _targetAngleY = 0.1f;
    }

    float posX = _mapCenter.x + _targetDistance * cosf(_targetAngleY) * cosf(_targetAngleXZ);
    float posY = _mapCenter.y + _targetDistance * sinf(_targetAngleY);
    float posZ = _mapCenter.z + _targetDistance * cosf(_targetAngleY) * sinf(_targetAngleXZ);

    this->_display->setCameraPosition({ posX, posY, posZ });
    this->_display->setCameraTarget(_mapCenter);
}

void CameraManager::setPlayerId(int playerId)
{
    _playerId = playerId;
}

int CameraManager::getPlayerId() const
{
    return _playerId;
}

void CameraManager::setGameInfos(std::shared_ptr<GameInfos> gameInfos)
{
    _gameInfos = gameInfos;
}

void CameraManager::handlePlayerCameraMouseInput()
{
    Vector2i screenSize = this->_display->getScreenSize();
    int screenCenterX = screenSize.x / 2;
    int screenCenterY = screenSize.y / 2;

    if (this->_display->isMouseButtonPressed(this->_display->getKeyId(MOUSE_RIGHT))) {
        _isPlayerViewDragging = true;
        this->_display->setMousePosition({static_cast<float>(screenCenterX),
            static_cast<float>(screenCenterY)});
        this->_display->disableCursor();
    }

    if (this->_display->isMouseButtonReleased(this->_display->getKeyId(MOUSE_RIGHT))) {
        _isPlayerViewDragging = false;
        this->_display->enableCursor();
    }

    if (_isPlayerViewDragging) {
        Vector2f mouseDelta = this->_display->getMouseDelta();
        const float rotationSensitivity = zappy::gui::CAMERA_SENSITIVITY;

        _playerAngleXZ += mouseDelta.x * rotationSensitivity;
        this->_display->setMousePosition({static_cast<float>(screenCenterX),
            static_cast<float>(screenCenterY)});
    }
}

Vector3f CameraManager::calculatePlayerPosition(const zappy::structs::Player& player)
{
    if (!_map) {
        const float basePlayerHeight = 0.2f;
        const float playerHeight = 0.5f;
        const float totalHeight = basePlayerHeight + playerHeight;

        return {
            static_cast<float>(player.x * zappy::gui::POSITION_MULTIPLIER),
            totalHeight,
            static_cast<float>(player.y * zappy::gui::POSITION_MULTIPLIER)
        };
    }

    Vector3f interpolatedPos = _map->getPlayerInterpolatedPosition(player.number,
        player.x, player.y);

    size_t playerIndex = 0;
    const auto& allPlayers = _gameInfos->getPlayers();

    for (const auto& p : allPlayers) {
        if (p.x == player.x && p.y == player.y) {
            if (p.number == player.number) {
                playerIndex = 0;
                break;
            }
            playerIndex++;
        }
    }

    float playerHeightOffset = _map->getOffset(DisplayPriority::PLAYER, player.x,
        player.y, playerIndex);
    float playerEntityHeight = 0.4f;
    float targetHeight = playerHeightOffset + playerEntityHeight / 2.0f;

    return {
        interpolatedPos.x,
        targetHeight,
        interpolatedPos.z
    };
}

Vector3f CameraManager::calculateCameraPosition(const Vector3f& playerPos, float angleXZ)
{
    const float fixedAngleY = 0.5f;
    const float fixedDistance = 6.0f;

    float posX = playerPos.x + fixedDistance * cosf(fixedAngleY) * cosf(angleXZ);
    float posY = playerPos.y + fixedDistance * sinf(fixedAngleY);
    float posZ = playerPos.z + fixedDistance * cosf(fixedAngleY) * sinf(angleXZ);

    return { posX, posY, posZ };
}

void CameraManager::updateCameraPlayerMode()
{
    this->_display->updateLastInputType();
    if (!_gameInfos || _playerId < 0 || !_map) {
        updateCameraFreeMode();
        return;
    }

    const auto& players = _gameInfos->getPlayers();
    auto playerIt = std::find_if(players.begin(), players.end(),
        [this](const zappy::structs::Player& player) {
            return player.number == _playerId;
        });

    if (playerIt == players.end()) {
        updateCameraFreeMode();
        return;
    }

    if (!_gameInfos->isTeamVisible(playerIt->teamName)) {
        updateCameraFreeMode();
        return;
    }

    handlePlayerCameraMouseInput();

    float deltaTime = this->_display->getFrameTime();
    if (this->_display->isKeyDown(this->_display->getKeyId(RIGHT)))
        _playerAngleXZ += this->_cameraRotaSpeed * deltaTime;

    if (this->_display->isKeyDown(this->_display->getKeyId(LEFT)))
        _playerAngleXZ -= this->_cameraRotaSpeed * deltaTime;

    if (this->_display->isGamepadAvailable()) {
        float rightStickX = this->_display->getGamepadAxisMovement(
            this->_display->getKeyId(GM_PD_AXIS_RIGHT_X));

        if (fabs(rightStickX) > zappy::gui::GAMEPAD_DEADZONE)
            _playerAngleXZ += rightStickX * zappy::gui::GAMEPAD_STICK_SENSITIVITY * deltaTime;
    }

    Vector3f playerPos = calculatePlayerPosition(*playerIt);
    Vector3f cameraPos = calculateCameraPosition(playerPos, _playerAngleXZ);

    this->_display->setCameraPosition(cameraPos);
    this->_display->setCameraTarget(playerPos);
}

void CameraManager::setMapCenter(const Vector3f &center)
{
    _mapCenter = Vector3f{
        center.x * zappy::gui::POSITION_MULTIPLIER,
        center.y,
        center.z * zappy::gui::POSITION_MULTIPLIER
    };
}

void CameraManager::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;

    if (_targetDistance <= 0.0f)
        _targetDistance = std::max(_mapWidth, _mapHeight) * 1.5f;
}

void CameraManager::setMapInstance(std::shared_ptr<Map> map)
{
    _map = map;
}

float CameraManager::getCameraMovingSpeed() {
    return this->_cameraMovingSpeed;
}

void CameraManager::setCameraMovingSpeed(float speed) {
    this->_cameraMovingSpeed = speed;
}

float CameraManager::getCameraRotaSpeed() {
    return this->_cameraRotaSpeed;
}

void CameraManager::setCameraRotaSpeed(float speed) {
    this->_cameraRotaSpeed = speed;
}

float CameraManager::getCameraZoomSpeed() {
    return this->_cameraZoomSpeed;
}

void CameraManager::setCameraZoomSpeed(float speed) {
    this->_cameraZoomSpeed = speed;
}
