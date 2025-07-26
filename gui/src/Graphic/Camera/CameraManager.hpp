/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CameraManager
*/

#ifndef CAMERA_MANAGER_HPP_
#define CAMERA_MANAGER_HPP_

#include <memory>
#include "../../Utils/Constants.hpp"
#include "../../Game/GameInfos.hpp"
#include "../Map.hpp"

class CameraManager {
    public:
        explicit CameraManager(std::shared_ptr<IDisplay> display);
        ~CameraManager();

        void updateCamera(zappy::gui::CameraMode mode);
        void updateCameraFreeMode();
        void updateCameraTargetMode();
        void updateCameraPlayerMode();

        void setMapCenter(const Vector3f &center);
        void setMapSize(int width, int height);

        void setTargetDistance(float distance);
        void initTargetPositionFromCurrentCamera();

        void setPlayerId(int playerId);
        int getPlayerId() const;
        void setGameInfos(std::shared_ptr<GameInfos> gameInfos);
        void setMapInstance(std::shared_ptr<Map> map);

        float getCameraMovingSpeed();
        void setCameraMovingSpeed(float);
        float getCameraRotaSpeed();
        void setCameraRotaSpeed(float);
        float getCameraZoomSpeed();
        void setCameraZoomSpeed(float);

        Vector3f calculatePlayerPosition(const zappy::structs::Player& player);
        Vector3f calculateCameraPosition(const Vector3f& playerPos, float angleXZ);

    private:
        float _cameraMovingSpeed = 15.0f;
        float _cameraRotaSpeed = 2.0f;
        float _cameraZoomSpeed = 120.0f;
        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<Map> _map;
        Vector3f _mapCenter;
        int _mapWidth;
        int _mapHeight;

        float _targetDistance;
        float _targetAngleXZ;
        float _targetAngleY;
        bool _isDragging;
        int _playerId;

        float _playerAngleXZ;
        bool _isPlayerViewDragging;

        void handlePlayerCameraMouseInput();
};

#endif /* !CAMERA_MANAGER_HPP_ */
