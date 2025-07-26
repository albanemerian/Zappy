/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos
*/

#ifndef GAMEINFOS_HPP_
#define GAMEINFOS_HPP_

#include <utility>
#include <vector>
#include <memory>
#include <mutex>
#include <string>
#include <chrono>
#include <unordered_map>

#include "../Utils/Constants.hpp"
#include "../Communication/ICommunication.hpp"
#include "../Observer/Subject.hpp"
#include "../Audio/IAudio.hpp"

class GameInfos : public Subject {
    public:
        explicit GameInfos(std::shared_ptr<ICommunication> communication);
        ~GameInfos();

        void setAudio(std::shared_ptr<IAudio> audio);
        void setCurrentCameraMode(zappy::gui::CameraMode cameraMode);
        void setCurrentPlayerFocus(int playerId);
        void setPerformanceMode(bool performanceMode);

        void setMapSize(int width, int height);
        std::pair<int, int> getMapSize() const;

        void setTimeUnit(int timeUnit, bool sendToServer = false);
        int getTimeUnit() const;

        void updateTile(const zappy::structs::Tile tile);
        const zappy::structs::Tile getTile(int x, int y) const;
        const zappy::structs::Tile& getTileRef(int x, int y) const;
        void initializeTileMatrix();

        void updateTeamName(const std::string &teamName);
        const std::vector<std::string> getTeamNames() const;

        void setTeamVisibility(const std::string &teamName, bool visible);
        bool isTeamVisible(const std::string &teamName) const;
        const std::unordered_map<std::string, bool> getTeamVisibilities() const;

        void setObjectVisibility(const std::string &objectType, bool visible);
        bool isObjectVisible(const std::string &objectType) const;
        const std::unordered_map<std::string, bool> getObjectVisibilities() const;

        Color32 getTeamColor(const std::string &teamName);

        void addPlayer(const zappy::structs::Player player);
        void killPlayer(int playerNumber);
        void updatePlayerPosition(int playerNumber, int x, int y);
        void updatePlayerOrientation(int playerNumber, int orientation);
        void updatePlayerLevel(int playerNumber, int level);
        void updatePlayerInventory(int playerNumber,
            const zappy::structs::Inventory inventory);
        void updatePlayerExpulsion(int playerNumber);
        void updatePlayerDeath(int playerNumber);
        void updatePlayerResourceAction(int playerNumber, int resourceId, bool isCollecting);
        void updatePlayerFork(int playerNumber);
        const std::vector<zappy::structs::Player> getPlayers() const;
        const zappy::structs::Player getPlayer(int playerNumber) const;

        void addPlayerBroadcast(int playerNumber, const std::string &message);
        const std::vector<std::pair<int, std::string>> getPlayersBroadcasting();

        void addIncantation(const zappy::structs::Incantation incantation);
        void removeIncantation(int x, int y, int result);
        const std::vector<zappy::structs::Incantation> getIncantations();

        void addEgg(const zappy::structs::Egg egg);
        void updateEggHatched(int eggNumber);
        void updateEggDeath(int eggNumber);
        const std::vector<zappy::structs::Egg> getEggs() const;

        void setGameOver(const std::string &winningTeam);
        void playDefeatSound(const std::string &teamName);
        std::pair<bool, std::string> isGameOver() const;

        void addServerMessage(const std::string &message);
        const std::vector<std::string> getServerMessages() const;

        void securityActualisation();
        void incrementPlayerLevel(int playerNumber);
        void decrementPlayerLevel(int playerNumber);
        void incrementPlayerInventoryItem(int playerNumber, int resourceId);
        void decrementPlayerInventoryItem(int playerNumber, int resourceId);
        void incrementTileInventoryItem(int x, int y, int resourceId);
        void decrementTileInventoryItem(int x, int y, int resourceId);

        void updateResourceTotals();
        int getTotalResource(const std::string& resourceName);
        int getTotalFood();
        int getTotalEggs() const;
        int getTotalLinemate();
        int getTotalDeraumere();
        int getTotalSibur();
        int getTotalMendiane();
        int getTotalPhiras();
        int getTotalThystame();

    private:
        int _mapWidth;
        int _mapHeight;
        int _timeUnit;

        std::vector<std::vector<zappy::structs::Tile>> _tileMatrix;
        bool _matrixInitialized;
        std::vector<std::string> _teamNames;
        std::unordered_map<std::string, bool> _teamVisibilities;
        std::unordered_map<std::string, Color32> _teamColors;
        std::unordered_map<std::string, bool> _objectVisibilities;
        std::vector<Color32> _colors;
        int _colorIndex = 0;
        std::vector<zappy::structs::Player> _players;
        std::vector<std::tuple<int, std::string, std::chrono::steady_clock::time_point>>
            _playersBroadcasting;
        std::vector<zappy::structs::Incantation> _incantations;
        std::vector<zappy::structs::Egg> _eggs;
        std::vector<std::string> _serverMessages;

        bool _gameOver;
        std::string _winningTeam;
        bool _victorySoundPlayed;

        mutable std::mutex _dataMutex;

        std::shared_ptr<ICommunication> _communication;
        std::shared_ptr<IAudio> _audio;
        zappy::gui::CameraMode _currentCameraMode;
        int _currentPlayerFocus;

        mutable std::unordered_map<std::string, int> _resourceTotals;
        mutable bool _resourceTotalsNeedUpdate = true;
        bool _performanceMode = false;

        void notifyStateChange();
};

#endif /* !GAMEINFOS_HPP_ */
