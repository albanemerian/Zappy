/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos
*/

#include <utility>
#include <vector>
#include <algorithm>
#include <string>
#include <mutex>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include "GameInfos.hpp"
#include "../Exceptions/Exceptions.hpp"

GameInfos::GameInfos(std::shared_ptr<ICommunication> communication) :
    _mapWidth(0),
    _mapHeight(0),
    _timeUnit(0),
    _matrixInitialized(false),
    _colorIndex(0),
    _gameOver(false),
    _victorySoundPlayed(false),
    _currentCameraMode(zappy::gui::CameraMode::FREE),
    _currentPlayerFocus(-1),
    _performanceMode(false)
{
    _communication = communication;
    _colors = {CBLUE, CYELLOW, CPURPLE, CORANGE, CPINK, CMAROON, CRED, CGREEN};
}

GameInfos::~GameInfos()
{
}

void GameInfos::setAudio(std::shared_ptr<IAudio> audio)
{
    _audio = audio;
}

void GameInfos::setCurrentCameraMode(zappy::gui::CameraMode cameraMode)
{
    _currentCameraMode = cameraMode;
}

void GameInfos::setCurrentPlayerFocus(int playerId)
{
    _currentPlayerFocus = playerId;
}

void GameInfos::setMapSize(int width, int height)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _mapWidth = width;
    _mapHeight = height;
    initializeTileMatrix();
}

void GameInfos::initializeTileMatrix()
{
    if (_mapWidth <= 0 || _mapHeight <= 0)
        return;

    _tileMatrix.clear();
    _tileMatrix.resize(_mapHeight);

    for (int y = 0; y < _mapHeight; ++y) {
        _tileMatrix[y].resize(_mapWidth);
        for (int x = 0; x < _mapWidth; ++x) {
            _tileMatrix[y][x] = zappy::structs::Tile(x, y);
        }
    }

    _matrixInitialized = true;
    _resourceTotalsNeedUpdate = true;
}

std::pair<int, int> GameInfos::getMapSize() const
{
    return std::make_pair(_mapWidth, _mapHeight);
}

void GameInfos::setTimeUnit(int timeUnit, bool sendToServer)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (sendToServer) {
        try {
            _communication->sendMessage("sst " + std::to_string(timeUnit) + "\n");
        } catch (const Exceptions::NetworkException& e) {
            std::cerr << colors::T_RED << "[ERROR] Network exception: "
                      << e.what() << colors::RESET << std::endl;
        }
    } else {
        _timeUnit = timeUnit;
    }
}

int GameInfos::getTimeUnit() const
{
    return _timeUnit;
}

void GameInfos::updateTile(const zappy::structs::Tile tile)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (tile.x < 0 || tile.y < 0 || tile.x >= _mapWidth || tile.y >= _mapHeight)
        return;

    if (!_matrixInitialized)
        initializeTileMatrix();

    _tileMatrix[tile.y][tile.x] = zappy::structs::Tile(
        tile.x, tile.y, tile.food, tile.linemate, tile.deraumere,
        tile.sibur, tile.mendiane, tile.phiras, tile.thystame);

    _resourceTotalsNeedUpdate = true;
}

const zappy::structs::Tile GameInfos::getTile(int x, int y) const
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (x < 0 || y < 0 || x >= _mapWidth || y >= _mapHeight || !_matrixInitialized)
        return zappy::structs::Tile(x, y);

    return _tileMatrix[y][x];
}

const zappy::structs::Tile& GameInfos::getTileRef(int x, int y) const
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (x < 0 || y < 0 || x >= _mapWidth || y >= _mapHeight || !_matrixInitialized) {
        static const zappy::structs::Tile defaultTile(0, 0);
        return defaultTile;
    }

    return _tileMatrix[y][x];
}

void GameInfos::updateTeamName(const std::string &teamName)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (std::find(_teamNames.begin(), _teamNames.end(), teamName) == _teamNames.end()) {
        _teamNames.push_back(teamName);
        _teamVisibilities[teamName] = true;
    }
}

const std::vector<std::string> GameInfos::getTeamNames() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _teamNames;
}

void GameInfos::setTeamVisibility(const std::string &teamName, bool visible)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _teamVisibilities[teamName] = visible;
}

bool GameInfos::isTeamVisible(const std::string &teamName) const
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    auto it = _teamVisibilities.find(teamName);
    if (it != _teamVisibilities.end()) {
        return it->second;
    }

    return true;
}

const std::unordered_map<std::string, bool> GameInfos::getTeamVisibilities() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _teamVisibilities;
}

void GameInfos::setObjectVisibility(const std::string &objectType, bool visible)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _objectVisibilities[objectType] = visible;
}

bool GameInfos::isObjectVisible(const std::string &objectType) const
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    auto it = _objectVisibilities.find(objectType);
    if (it != _objectVisibilities.end()) {
        return it->second;
    }

    return true;
}

const std::unordered_map<std::string, bool> GameInfos::getObjectVisibilities() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _objectVisibilities;
}

Color32 GameInfos::getTeamColor(const std::string &teamName)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (teamName.empty())
        return CWHITE;

    if (_teamColors.find(teamName) == _teamColors.end()) {
        _teamColors[teamName] = _colors[_colorIndex];
        _colorIndex = (_colorIndex + 1) % _colors.size();
    }
    return _teamColors[teamName];
}

void GameInfos::addPlayer(const zappy::structs::Player player)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    auto it = std::find_if(_players.begin(), _players.end(),
                           [&player](const zappy::structs::Player &p) {
                               return p.number == player.number;
                           });

    if (it != _players.end())
        *it = player;
    else {
        _players.push_back(zappy::structs::Player(
            player.number, player.x, player.y, player.orientation,
            player.level, player.teamName, player.inventory));
    }

    try {
        _communication->sendMessage("pin #" + std::to_string(player.number) + "\n");
    } catch (const Exceptions::NetworkException& e) {
        std::cerr << colors::T_RED << "[ERROR] Network exception: "
                  << e.what() << colors::RESET << std::endl;
    }
}

void GameInfos::killPlayer(int playerNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (playerNumber < 0)
        return;

    try {
        _communication->sendMessage("kil #" + std::to_string(playerNumber) + "\n");
    } catch (const Exceptions::NetworkException& e) {
        std::cerr << colors::T_RED << "[ERROR] Network exception: "
                  << e.what() << colors::RESET << std::endl;
    }
}

void GameInfos::updatePlayerPosition(int playerNumber, int x, int y)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (x < 0 || y < 0 || x >= _mapWidth || y >= _mapHeight)
        return;

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            player.x = x;
            player.y = y;
            return;
        }
    }
}

void GameInfos::updatePlayerOrientation(int playerNumber, int orientation)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (orientation < 1 || orientation > 4)
        return;

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            player.orientation = orientation;
            return;
        }
    }
}

void GameInfos::updatePlayerLevel(int playerNumber, int level)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            player.level = level;
            return;
        }
    }
}

void GameInfos::incrementPlayerLevel(int playerNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            if (player.level < 8) {
                try {
                    _communication->sendMessage("plu #" + std::to_string(playerNumber) + "\n");
                } catch (const Exceptions::NetworkException& e) {
                    std::cerr << colors::T_RED << "[ERROR] Network exception: "
                              << e.what() << colors::RESET << std::endl;
                    return;
                }
                return;
            }
            return;
        }
    }
    return;
}

void GameInfos::decrementPlayerLevel(int playerNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            if (player.level > 1) {
                try {
                    _communication->sendMessage("pld #" + std::to_string(playerNumber) + "\n");
                } catch (const Exceptions::NetworkException& e) {
                    std::cerr << colors::T_RED << "[ERROR] Network exception: "
                              << e.what() << colors::RESET << std::endl;
                    return;
                }
                return;
            }
            return;
        }
    }
    return;
}

void GameInfos::updatePlayerInventory(int playerNumber,
    const zappy::structs::Inventory inventory)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            player.inventory = zappy::structs::Inventory(
                inventory.food, inventory.linemate, inventory.deraumere,
                inventory.sibur, inventory.mendiane, inventory.phiras,
                inventory.thystame);
            return;
        }
    }
}

void GameInfos::updatePlayerExpulsion(int playerNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (playerNumber < 0)
        return;

    if (_audio)
        _audio->playSound("zap", this->_audio->getSFXVolumeLevel());
}

void GameInfos::updatePlayerDeath(int playerNumber)
{
    try {
        std::unique_lock<std::mutex> lock(_dataMutex);

        if (playerNumber < 0)
            return;

        std::string teamName;
        auto it = std::find_if(_players.begin(), _players.end(),
            [playerNumber](const zappy::structs::Player &player) {
                return player.number == playerNumber;
            });

        if (it != _players.end()) {
            teamName = it->teamName;
        } else {
            return;
        }

        _players.erase(std::remove_if(_players.begin(), _players.end(),
            [playerNumber](const zappy::structs::Player &player) {
                return player.number == playerNumber;
            }), _players.end());

        if (!teamName.empty()) {
            int remainingPlayers = std::count_if(_players.begin(), _players.end(),
                [&teamName](const zappy::structs::Player &player) {
                    return player.teamName == teamName;
                });

            if (remainingPlayers == 0 && !_gameOver) {
                std::string eliminatedTeam = teamName;
                lock.unlock();
                playDefeatSound(eliminatedTeam);
                return;
            }
        }
    } catch (const std::exception& e) {
        std::cout << colors::T_RED << "[ERROR] Exception in updatePlayerDeath: "
                  << e.what() << colors::RESET << std::endl;
    } catch (...) {
        std::cout << colors::T_RED << "[ERROR] Unknown exception in updatePlayerDeath"
                  << colors::RESET << std::endl;
    }
}

void GameInfos::updatePlayerResourceAction(int playerNumber, int resourceId, bool isCollecting)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (playerNumber < 0 || resourceId < 0 || resourceId > 6)
        return;

    if (isCollecting && _audio) {
        if (_currentCameraMode == zappy::gui::CameraMode::PLAYER) {
            if (playerNumber == _currentPlayerFocus) {
                _audio->playSound("collect", this->_audio->getSFXVolumeLevel());
            }
        }
    }
}

void GameInfos::updatePlayerFork(int playerNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    (void)playerNumber;
}

const std::vector<zappy::structs::Player> GameInfos::getPlayers() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _players;
}

void GameInfos::addPlayerBroadcast(int playerNumber, const std::string &message)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    if (playerNumber < 0 || message.empty())
        return;

    auto currentTime = std::chrono::steady_clock::now();
    _playersBroadcasting.emplace_back(playerNumber, message, currentTime);
}

const std::vector<std::pair<int, std::string>> GameInfos::getPlayersBroadcasting()
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    std::vector<std::pair<int, std::string>> result;
    for (const auto& broadcast : _playersBroadcasting) {
        result.emplace_back(std::get<0>(broadcast), std::get<1>(broadcast));
    }

    _playersBroadcasting.clear();
    return result;
}

void GameInfos::addIncantation(const zappy::structs::Incantation incantation)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _incantations.push_back(zappy::structs::Incantation(
        incantation.x, incantation.y, incantation.level, incantation.players));
}

void GameInfos::removeIncantation(int x, int y, int result)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    auto it = std::remove_if(_incantations.begin(), _incantations.end(),
                             [x, y](const zappy::structs::Incantation &inc) {
                                 return inc.x == x && inc.y == y;
                             });

    if (it != _incantations.end())
        _incantations.erase(it, _incantations.end());

    (void)result;
}

const std::vector<zappy::structs::Incantation> GameInfos::getIncantations()
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    _incantations.erase(
        std::remove_if(_incantations.begin(), _incantations.end(),
            [this](const zappy::structs::Incantation& inc) {
                for (int playerNum : inc.players) {
                    auto it = std::find_if(_players.begin(), _players.end(),
                        [playerNum](const zappy::structs::Player& p) {
                            return p.number == playerNum;
                        });
                    if (it == _players.end())
                        return true;
                }
                return false;
            }),
        _incantations.end());
    return _incantations;
}

void GameInfos::addEgg(const zappy::structs::Egg egg)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    auto it = std::find_if(_eggs.begin(), _eggs.end(),
                           [&egg](const zappy::structs::Egg &e) {
                               return e.eggNumber == egg.eggNumber;
                           });

    if (it != _eggs.end())
        *it = egg;
    else {
        _eggs.push_back(zappy::structs::Egg(
            egg.eggNumber, egg.playerNumber, egg.x, egg.y, egg.hatched, egg.teamName));
    }
}

void GameInfos::updateEggHatched(int eggNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    for (auto &egg : _eggs) {
        if (egg.eggNumber == eggNumber) {
            egg.hatched = true;
            return;
        }
    }
}

void GameInfos::updateEggDeath(int eggNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _eggs.erase(std::remove_if(_eggs.begin(), _eggs.end(),
        [eggNumber](const zappy::structs::Egg &egg) {
            return egg.eggNumber == eggNumber;
        }), _eggs.end());
}

const std::vector<zappy::structs::Egg> GameInfos::getEggs() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _eggs;
}

void GameInfos::setGameOver(const std::string &winningTeam)
{
    try {
        std::lock_guard<std::mutex> lock(_dataMutex);
        _gameOver = true;
        _winningTeam = winningTeam;

        if (_audio && !_victorySoundPlayed) {
            _audio->playSound("win", 100.0f);
            _victorySoundPlayed = true;
        }
    } catch (const std::exception& e) {
        std::cout << colors::T_RED << "[ERROR] Exception in setGameOver: "
                  << e.what() << colors::RESET << std::endl;
    } catch (...) {
        std::cout << colors::T_RED << "[ERROR] Unknown exception in setGameOver"
                  << colors::RESET << std::endl;
    }

    notifyGameEvent(GameEventType::TEAM_WIN, winningTeam);
}

void GameInfos::playDefeatSound(const std::string &teamName)
{
    try {
        std::lock_guard<std::mutex> lock(_dataMutex);

        if (_audio) {
            _audio->playSound("loose", 100.0f);
        }

        std::cout << colors::T_RED << "[INFO] Team " << teamName << " has been eliminated"
                  << colors::RESET << std::endl;
    } catch (const std::exception& e) {
        std::cout << colors::T_RED << "[ERROR] Exception in playDefeatSound: "
                  << e.what() << colors::RESET << std::endl;
    } catch (...) {
        std::cout << colors::T_RED << "[ERROR] Unknown exception in playDefeatSound"
                  << colors::RESET << std::endl;
    }
    notifyGameEvent(GameEventType::TEAM_DEFEAT, teamName);
}

std::pair<bool, std::string> GameInfos::isGameOver() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return std::make_pair(_gameOver, _winningTeam);
}

const zappy::structs::Player GameInfos::getPlayer(int playerNumber) const
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (const auto &player : _players) {
        if (player.number == playerNumber)
            return player;
    }
    return zappy::structs::Player();
}

void GameInfos::notifyStateChange() {
    notifyObservers();
}

void GameInfos::addServerMessage(const std::string &message)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _serverMessages.push_back(message);
}

const std::vector<std::string> GameInfos::getServerMessages() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _serverMessages;
}

void GameInfos::securityActualisation()
{
    std::cout << colors::T_BLUE << "[INFO] Performing security actualization..."
              << colors::RESET << std::endl;

    try {
        std::lock_guard<std::mutex> lock(_dataMutex);

        _communication->sendMessage("msz\n");
        _communication->sendMessage("tna\n");
        _communication->sendMessage("sgt\n");
        _communication->sendMessage("mct\n");

        for (const auto& player : _players) {
            _communication->sendMessage("ppo #" + std::to_string(player.number) + "\n");
            _communication->sendMessage("plv #" + std::to_string(player.number) + "\n");
            _communication->sendMessage("pin #" + std::to_string(player.number) + "\n");
        }
    } catch (const Exceptions::NetworkException& e) {
        std::cerr << colors::T_RED << "[ERROR] Network exception: "
                  << e.what() << colors::RESET << std::endl;
    }
}

void GameInfos::incrementPlayerInventoryItem(int playerNumber, int resourceId)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (playerNumber < 0 || resourceId < 0 || resourceId > 6)
        return;

    try {
        _communication->sendMessage("pia #" + std::to_string(playerNumber) + " " +
                                    std::to_string(resourceId) + "\n");
    } catch (const Exceptions::NetworkException& e) {
        std::cerr << colors::T_RED << "[ERROR] Network exception: "
                  << e.what() << colors::RESET << std::endl;
    }
}

void GameInfos::decrementPlayerInventoryItem(int playerNumber, int resourceId)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (playerNumber < 0 || resourceId < 0 || resourceId > 6)
        return;

    try {
        _communication->sendMessage("pis #" + std::to_string(playerNumber) + " " +
                                    std::to_string(resourceId) + "\n");
    } catch (const Exceptions::NetworkException& e) {
        std::cerr << colors::T_RED << "[ERROR] Network exception: "
                  << e.what() << colors::RESET << std::endl;
    }
}

void GameInfos::incrementTileInventoryItem(int x, int y, int resourceId)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (x < 0 || y < 0 || resourceId < 0 || resourceId > 6)
        return;

    try {
        _communication->sendMessage("tar " + std::to_string(x) + " " +
                                    std::to_string(y) + " " +
                                    std::to_string(resourceId) + "\n");

        _resourceTotalsNeedUpdate = true;
    } catch (const Exceptions::NetworkException& e) {
        std::cerr << colors::T_RED << "[ERROR] Network exception: "
                  << e.what() << colors::RESET << std::endl;
    }
}

void GameInfos::decrementTileInventoryItem(int x, int y, int resourceId)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (x < 0 || y < 0 || resourceId < 0 || resourceId > 6)
        return;

    try {
        _communication->sendMessage("tsr " + std::to_string(x) + " " +
                                    std::to_string(y) + " " +
                                    std::to_string(resourceId) + "\n");

        _resourceTotalsNeedUpdate = true;
    } catch (const Exceptions::NetworkException& e) {
        std::cerr << colors::T_RED << "[ERROR] Network exception: "
                  << e.what() << colors::RESET << std::endl;
    }
}

void GameInfos::updateResourceTotals()
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (!_resourceTotalsNeedUpdate || _performanceMode)
        return;
    _resourceTotals["food"] = 0;
    _resourceTotals["linemate"] = 0;
    _resourceTotals["deraumere"] = 0;
    _resourceTotals["sibur"] = 0;
    _resourceTotals["mendiane"] = 0;
    _resourceTotals["phiras"] = 0;
    _resourceTotals["thystame"] = 0;

    if (!_matrixInitialized)
        return;

    for (int y = 0; y < _mapHeight; y++) {
        for (int x = 0; x < _mapWidth; x++) {
            auto& tile = _tileMatrix[y][x];
            _resourceTotals["food"] += tile.food;
            _resourceTotals["linemate"] += tile.linemate;
            _resourceTotals["deraumere"] += tile.deraumere;
            _resourceTotals["sibur"] += tile.sibur;
            _resourceTotals["mendiane"] += tile.mendiane;
            _resourceTotals["phiras"] += tile.phiras;
            _resourceTotals["thystame"] += tile.thystame;
        }
    }

    _resourceTotalsNeedUpdate = false;
}

int GameInfos::getTotalResource(const std::string& resourceName)
{
    updateResourceTotals();

    auto it = _resourceTotals.find(resourceName);
    if (it != _resourceTotals.end()) {
        return it->second;
    }

    return 0;
}

int GameInfos::getTotalFood()
{
    return getTotalResource("food");
}

int GameInfos::getTotalEggs() const
{
    return _eggs.size();
}

int GameInfos::getTotalLinemate()
{
    return getTotalResource("linemate");
}

int GameInfos::getTotalDeraumere()
{
    return getTotalResource("deraumere");
}

int GameInfos::getTotalSibur()
{
    return getTotalResource("sibur");
}

int GameInfos::getTotalMendiane()
{
    return getTotalResource("mendiane");
}

int GameInfos::getTotalPhiras()
{
    return getTotalResource("phiras");
}

int GameInfos::getTotalThystame()
{
    return getTotalResource("thystame");
}

void GameInfos::setPerformanceMode(bool performanceMode)
{
    _performanceMode = performanceMode;
}
