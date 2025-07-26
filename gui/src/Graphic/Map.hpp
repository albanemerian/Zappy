/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Map
*/

#ifndef MAP_HPP_
#define MAP_HPP_

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include "../Game/GameInfos.hpp"
#include "../IDisplay.hpp"

enum class DisplayPriority {
    TILE = 0,
    EGG = 1,
    PLAYER = 2,
    FOOD = 3,
    ROCK = 4,
};

struct PlayerRotationState {
    float currentRotation;
    float targetRotation;
    bool isRotating;
    std::chrono::steady_clock::time_point lastUpdateTime;

    PlayerRotationState() : currentRotation(0.0f), targetRotation(0.0f),
                    isRotating(false), lastUpdateTime(std::chrono::steady_clock::now()) {}
};

struct PlayerPositionState {
    Vector3f currentPosition;
    Vector3f targetPosition;
    bool isMoving;
    std::chrono::steady_clock::time_point lastUpdateTime;

    PlayerPositionState() : currentPosition({0.0f, 0.0f, 0.0f}),
                    targetPosition({0.0f, 0.0f, 0.0f}),
                    isMoving(false), lastUpdateTime(std::chrono::steady_clock::now()) {}
};

class Map {
    public:
        Map(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<IDisplay> display);
        ~Map();

        void draw(bool performanceMode = false);
        void drawBroadcastingPlayers();
        void drawIncantations();

        void drawTile(int x, int y, const zappy::structs::Tile &tile);
        void drawPerformanceTile(const zappy::structs::Tile &tile);

        void drawRock(int x, int y, const zappy::structs::Tile &tile);
        void drawPerformanceRock(int x, int y, const zappy::structs::Tile &tile);

        void drawFood(int x, int y, const zappy::structs::Tile &tile);
        void drawPerformanceFood(int x, int y, const zappy::structs::Tile &tile);

        void drawAllPlayers();
        void drawEggs(int x, int y);

        float getOffset(DisplayPriority priority, int x, int y, size_t stackIndex = 0);
        void updatePlayerRotations();
        float getPlayerInterpolatedRotation(int playerId, int serverOrientation);
        void updatePlayerPositions();
        Vector3f getPlayerInterpolatedPosition(int playerId, int serverX, int serverY);

    private:
        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<IDisplay> _display;

        std::unordered_map<int, std::chrono::steady_clock::time_point> _broadcastStartTimes;
        std::unordered_map<int, PlayerRotationState> _playerRotations;
        std::unordered_map<int, PlayerPositionState> _playerPositions;

        mutable std::mutex _playerStatesMutex;

        static constexpr float BASE_HEIGHT_TILE = 0.0f;

        static constexpr float BASE_HEIGHT_PLAYER = 0.0f;
        static constexpr float PLAYER_HEIGHT = 0.95f;

        static constexpr float BASE_HEIGHT_EGG = 0.0f;
        static constexpr float EGG_HEIGHT = 0.2f;

        static constexpr float BASE_HEIGHT_FOOD = 0.1f;
        static constexpr float FOOD_HEIGHT = 0.7f;

        static constexpr float BASE_HEIGHT_ROCK = 0.1f;
        static constexpr float ROCK_HEIGHT = 0.7f;


        void drawTorus(const Vector3f &position, float radius, float thickness,
            int radialSegments, Color32 color);
        float orientationToRotation(int orientation);
        float normalizeAngle(float angle);
        float getShortestAngleDifference(float from, float to);
        Vector3f calculatePlayerWorldPosition(int x, int y);
        float getDistance(const Vector3f& from, const Vector3f& to);
        Vector3f lerpVector3f(const Vector3f& from, const Vector3f& to, float t);

        bool _performanceMode = false;
};

#endif /* !MAP_HPP_ */
