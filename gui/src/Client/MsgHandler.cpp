/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** MsgHandler
*/

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

#include "../Exceptions/Exceptions.hpp"
#include "MsgHandler.hpp"

MsgHandler::MsgHandler(std::shared_ptr<GameInfos> gameInfos,
    std::shared_ptr<ICommunication> communication)
    : _running(false),
    _gameInfos(gameInfos),
    _communication(communication)
{
    _messageHandlers = {
        {"WELCOME", std::bind(&MsgHandler::handleWelcomeMessage, this, std::placeholders::_1)},
        {"msz", std::bind(&MsgHandler::handleMszMessage, this, std::placeholders::_1)},
        {"bct", std::bind(&MsgHandler::handleBctMessage, this, std::placeholders::_1)},
        {"tna", std::bind(&MsgHandler::handleTnaMessage, this, std::placeholders::_1)},
        {"pnw", std::bind(&MsgHandler::handlePnwMessage, this, std::placeholders::_1)},
        {"ppo", std::bind(&MsgHandler::handlePpoMessage, this, std::placeholders::_1)},
        {"plv", std::bind(&MsgHandler::handlePlvMessage, this, std::placeholders::_1)},
        {"pin", std::bind(&MsgHandler::handlePinMessage, this, std::placeholders::_1)},
        {"pex", std::bind(&MsgHandler::handlePexMessage, this, std::placeholders::_1)},
        {"pbc", std::bind(&MsgHandler::handlePbcMessage, this, std::placeholders::_1)},
        {"pic", std::bind(&MsgHandler::handlePicMessage, this, std::placeholders::_1)},
        {"pie", std::bind(&MsgHandler::handlePieMessage, this, std::placeholders::_1)},
        {"pfk", std::bind(&MsgHandler::handlePfkMessage, this, std::placeholders::_1)},
        {"pdr", std::bind(&MsgHandler::handlePdrMessage, this, std::placeholders::_1)},
        {"pgt", std::bind(&MsgHandler::handlePgtMessage, this, std::placeholders::_1)},
        {"pdi", std::bind(&MsgHandler::handlePdiMessage, this, std::placeholders::_1)},
        {"enw", std::bind(&MsgHandler::handleEnwMessage, this, std::placeholders::_1)},
        {"ebo", std::bind(&MsgHandler::handleEboMessage, this, std::placeholders::_1)},
        {"edi", std::bind(&MsgHandler::handleEdiMessage, this, std::placeholders::_1)},
        {"sgt", std::bind(&MsgHandler::handleSgtMessage, this, std::placeholders::_1)},
        {"sst", std::bind(&MsgHandler::handleSstMessage, this, std::placeholders::_1)},
        {"seg", std::bind(&MsgHandler::handleSegMessage, this, std::placeholders::_1)},
        {"smg", std::bind(&MsgHandler::handleSmgMessage, this, std::placeholders::_1)},
        {"suc", std::bind(&MsgHandler::handleSucMessage, this, std::placeholders::_1)},
        {"sbp", std::bind(&MsgHandler::handleSbpMessage, this, std::placeholders::_1)}
    };

    start();
}

MsgHandler::~MsgHandler()
{
    stop();
}

void MsgHandler::start()
{
    if (!_running) {
        _running = true;
        _thread = std::thread(&MsgHandler::messageLoop, this);
    }
}

void MsgHandler::stop()
{
    if (_running) {
        _running = false;
        _condition.notify_one();

        if (_thread.joinable())
            _thread.join();
    }
}

void MsgHandler::messageLoop()
{
    while (_running) {
        if (_communication->hasMessages()) {
            handleMessage(_communication->popMessage());

        } else {
            std::unique_lock<std::mutex> lock(_mutex);

            _condition.wait_for(lock, std::chrono::milliseconds(100), [this] {
                return !_running || _communication->hasMessages();
            });
        }
    }
}

void MsgHandler::handleMessage(const std::string& message)
{
    if (message.empty())
        return;

    for (auto &handler : _messageHandlers) {
        try {
            if (message.find(handler.first) != std::string::npos && handler.second(message))
                return;
        } catch (const Exceptions::NetworkException& e) {
            std::cerr << colors::T_RED << "[ERROR] Network exception: " << e.what()
                      << colors::RESET << std::endl;
            _communication->disconnect();
            return;
        }
    }
    std::cout << colors::T_RED << "[WARNING] Unhandled message: " << message
              << colors::RESET << std::endl;
}

bool MsgHandler::handleWelcomeMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::cout << colors::T_YELLOW << "[INFO] Welcome message received: " << message
              << colors::RESET << std::endl;
    _communication->sendMessage("GRAPHIC\n");
    std::cout << colors::T_YELLOW << "[INFO] Sent GRAPHIC command to server."
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleMszMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int width, height;

    iss >> prefix >> width >> height;

    if (iss.fail() || width <= 0 || height <= 0 || prefix != "msz") {
        std::cerr << colors::T_RED << "[WARNING] Invalid map size format received: " << message
                  << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->setMapSize(width, height);
    }

    std::cout << colors::T_YELLOW << "[INFO] Map size set to: " << width << "x" << height
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleSgtMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int timeUnit;

    iss >> prefix >> timeUnit;

    if (iss.fail() || timeUnit <= 0 || prefix != "sgt") {
        std::cerr << colors::T_RED << "[WARNING] Invalid time unit format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->setTimeUnit(timeUnit, false);
    }

    std::cout << colors::T_YELLOW << "[INFO] Time unit set to: " << timeUnit
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleBctMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame;

    iss >> prefix >> x >> y >> food >> linemate >> deraumere
        >> sibur >> mendiane >> phiras >> thystame;

    if (iss.fail() || prefix != "bct" || x < 0 || y < 0 ||
        food < 0 || linemate < 0 || deraumere < 0 ||
        sibur < 0 || mendiane < 0 || phiras < 0 || thystame < 0) {
        std::cerr << colors::T_RED << "[WARNING] Invalid tile data format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    zappy::structs::Tile tile(x, y, food, linemate, deraumere,
                              sibur, mendiane, phiras, thystame);

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updateTile(tile);
    }

    std::cout << colors::T_YELLOW << "[INFO] Tile updated: (" << x << ", " << y << ") "
              << "Food: " << food << ", Linemate: " << linemate
              << ", Deraumere: " << deraumere << ", Sibur: " << sibur
              << ", Mendiane: " << mendiane << ", Phiras: " << phiras
              << ", Thystame: " << thystame
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleTnaMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, teamName;

    iss >> prefix >> teamName;

    if (iss.fail() || prefix != "tna" || teamName.empty()) {
        std::cerr << colors::T_RED << "[WARNING] Invalid team name format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updateTeamName(teamName);
    }

    std::cout << colors::T_YELLOW << "[INFO] Team name added: " << teamName
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePnwMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr, teamName;
    int x, y, orientation, level;

    iss >> prefix >> playerNumberStr >> x >> y >> orientation >> level >> teamName;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "pnw" || playerNumber < 0 || x < 0 || y < 0 ||
        orientation < 1 || orientation > 4 || level < 1 || teamName.empty()) {
        std::cerr << colors::T_RED << "[WARNING] Invalid player data format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        auto teams = _gameInfos->getTeamNames();
        if (std::find(teams.begin(), teams.end(), teamName) == teams.end()) {
            std::cerr << colors::T_RED << "[WARNING] Team name not found: " << teamName
                      << colors::RESET << std::endl;
            return false;
        }
    }

    zappy::structs::Player player(playerNumber, x, y, orientation, level, teamName);
    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->addPlayer(player);
    }

    std::cout << colors::T_YELLOW << "[INFO] Player added: " << playerNumber
              << " at (" << x << ", " << y << ") with orientation "
              << orientation << " and level " << level
              << " from team " << teamName
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePpoMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr;
    int x, y, orientation;

    iss >> prefix >> playerNumberStr >> x >> y >> orientation;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "ppo" || playerNumber < 0 || x < 0 || y < 0 ||
        orientation < 1 || orientation > 4) {
        std::cerr << colors::T_RED << "[WARNING] Invalid player position format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updatePlayerPosition(playerNumber, x, y);
        _gameInfos->updatePlayerOrientation(playerNumber, orientation);
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " position updated to (" << x << ", " << y
              << ") orientation " << orientation
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePlvMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr;
    int level;

    iss >> prefix >> playerNumberStr >> level;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "plv" || playerNumber < 0 || level < 1) {
        std::cerr << colors::T_RED << "[WARNING] Invalid player level format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updatePlayerLevel(playerNumber, level);
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " level updated to " << level
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePinMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr;
    int x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame;

    iss >> prefix >> playerNumberStr >> x >> y >> food >> linemate
        >> deraumere >> sibur >> mendiane >> phiras >> thystame;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "pin" || playerNumber < 0 || x < 0 || y < 0 ||
        food < 0 || linemate < 0 || deraumere < 0 ||
        sibur < 0 || mendiane < 0 || phiras < 0 || thystame < 0) {
        std::cerr << colors::T_RED << "[WARNING] Invalid player inventory format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    zappy::structs::Inventory inventory(food, linemate, deraumere,
                                        sibur, mendiane, phiras, thystame);
    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updatePlayerInventory(playerNumber, inventory);
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " inventory updated: Food: " << food
              << ", Linemate: " << linemate
              << ", Deraumere: " << deraumere
              << ", Sibur: " << sibur
              << ", Mendiane: " << mendiane
              << ", Phiras: " << phiras
              << ", Thystame: " << thystame
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePexMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr;

    iss >> prefix >> playerNumberStr;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "pex" || playerNumber < 0) {
        std::cerr << colors::T_RED << "[WARNING] Invalid player expulsion format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updatePlayerExpulsion(playerNumber);
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " has been expelled."
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePbcMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr, broadcastMessage;

    iss >> prefix >> playerNumberStr;
    std::getline(iss, broadcastMessage);

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "pbc" || playerNumber < 0 || broadcastMessage.empty()) {
        std::cerr << colors::T_RED << "[WARNING] Invalid player broadcast format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->addPlayerBroadcast(playerNumber, broadcastMessage);
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " broadcasted: " << broadcastMessage
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePicMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int x, y, level;
    std::vector<int> players;
    std::string playerStr;

    iss >> prefix >> x >> y >> level;
    if (iss.fail() || prefix != "pic" || x < 0 || y < 0 || level < 1) {
        std::cerr << colors::T_RED << "[WARNING] Invalid incantation format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    while (iss >> playerStr) {
        if (playerStr.length() > 1 && playerStr[0] == '#') {
            try {
                int playerNumber = std::stoi(playerStr.substr(1));
                players.push_back(playerNumber);
            } catch (const std::exception& e) {
                std::cerr << colors::T_RED
                          << "[WARNING] Invalid player number in incantation: "
                          << playerStr << colors::RESET << std::endl;
                return false;
            }
        } else {
            std::cerr << colors::T_RED
                      << "[WARNING] Invalid player number format in incantation: "
                      << playerStr << colors::RESET << std::endl;
            return false;
        }
    }

    zappy::structs::Incantation incantation(x, y, level, players);
    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->addIncantation(incantation);
    }

    std::cout << colors::T_YELLOW << "[INFO] Incantation at (" << x << ", " << y
              << ") for level " << level << " with players: ";
    for (const auto &playerNum : players) {
        std::cout << playerNum << " ";
    }
    std::cout << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePieMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int x, y, result;

    iss >> prefix >> x >> y >> result;
    if (iss.fail() || prefix != "pie" || x < 0 || y < 0 || (result != 0 && result != 1)) {
        std::cerr << colors::T_RED << "[WARNING] Invalid incantation result format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->removeIncantation(x, y, result);
    }

    std::cout << colors::T_YELLOW << "[INFO] Incantation at (" << x << ", " << y
              << ") result: " << (result == 1 ? "Success" : "Failure")
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePfkMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr;

    iss >> prefix >> playerNumberStr;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "pfk" || playerNumber < 0) {
        std::cerr << colors::T_RED << "[WARNING] Invalid player fork format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updatePlayerFork(playerNumber);
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " is laying an egg."
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePdrMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr;
    int resourceId;

    iss >> prefix >> playerNumberStr >> resourceId;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "pdr" || playerNumber < 0 || resourceId < 0
        || resourceId > 6) {
        std::cerr << colors::T_RED << "[WARNING] Invalid resource dropping format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updatePlayerResourceAction(playerNumber, resourceId, false);
    }

    std::string resourceName;
    switch(resourceId) {
        case 0: resourceName = "food"; break;
        case 1: resourceName = "linemate"; break;
        case 2: resourceName = "deraumere"; break;
        case 3: resourceName = "sibur"; break;
        case 4: resourceName = "mendiane"; break;
        case 5: resourceName = "phiras"; break;
        case 6: resourceName = "thystame"; break;
        default: resourceName = "unknown"; break;
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " dropped resource: " << resourceName
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePgtMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr;
    int resourceId;

    iss >> prefix >> playerNumberStr >> resourceId;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "pgt" || playerNumber < 0 || resourceId < 0
        || resourceId > 6) {
        std::cerr << colors::T_RED << "[WARNING] Invalid resource collecting format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updatePlayerResourceAction(playerNumber, resourceId, true);
    }

    std::string resourceName;
    switch(resourceId) {
        case 0: resourceName = "food"; break;
        case 1: resourceName = "linemate"; break;
        case 2: resourceName = "deraumere"; break;
        case 3: resourceName = "sibur"; break;
        case 4: resourceName = "mendiane"; break;
        case 5: resourceName = "phiras"; break;
        case 6: resourceName = "thystame"; break;
        default: resourceName = "unknown"; break;
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " collected resource: " << resourceName
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePdiMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, playerNumberStr;

    iss >> prefix >> playerNumberStr;

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "pdi" || playerNumber < 0) {
        std::cerr << colors::T_RED << "[WARNING] Invalid player death format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updatePlayerDeath(playerNumber);
    }

    std::cout << colors::T_YELLOW << "[INFO] Player " << playerNumber
              << " has died."
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleEnwMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, eggNumberStr, playerNumberStr;
    int x, y;

    iss >> prefix >> eggNumberStr >> playerNumberStr >> x >> y;

    int eggNumber = -1;
    if (eggNumberStr.length() > 1 && eggNumberStr[0] == '#') {
        try {
            eggNumber = std::stoi(eggNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid egg number format: "
                      << eggNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid egg number format: "
                  << eggNumberStr << colors::RESET << std::endl;
        return false;
    }

    int playerNumber = -1;
    if (playerNumberStr.length() > 1 && playerNumberStr[0] == '#') {
        try {
            playerNumber = std::stoi(playerNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                      << playerNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid player number format: "
                  << playerNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "enw" || eggNumber < 0 || playerNumber < -1
        || x < 0 || y < 0) {
        std::cerr << colors::T_RED << "[WARNING] Invalid egg laying format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    std::string teamName = "";
    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        auto players = _gameInfos->getPlayers();
        for (const auto &player : players) {
            if (player.number == playerNumber) {
                teamName = player.teamName;
                break;
            }
        }

        zappy::structs::Egg egg(eggNumber, playerNumber, x, y, false, teamName);
        _gameInfos->addEgg(egg);
    }

    std::cout << colors::T_YELLOW << "[INFO] Egg " << eggNumber
              << " laid by player " << playerNumber
              << " at (" << x << ", " << y << ") from team " << teamName
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleEboMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, eggNumberStr;

    iss >> prefix >> eggNumberStr;

    int eggNumber = -1;
    if (eggNumberStr.length() > 1 && eggNumberStr[0] == '#') {
        try {
            eggNumber = std::stoi(eggNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid egg number format: "
                      << eggNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid egg number format: "
                  << eggNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "ebo" || eggNumber < 0) {
        std::cerr << colors::T_RED << "[WARNING] Invalid egg hatching format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updateEggHatched(eggNumber);
    }

    std::cout << colors::T_YELLOW << "[INFO] Egg " << eggNumber
              << " has hatched and a player has connected."
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleEdiMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, eggNumberStr;

    iss >> prefix >> eggNumberStr;

    int eggNumber = -1;
    if (eggNumberStr.length() > 1 && eggNumberStr[0] == '#') {
        try {
            eggNumber = std::stoi(eggNumberStr.substr(1));
        } catch (const std::exception& e) {
            std::cerr << colors::T_RED << "[WARNING] Invalid egg number format: "
                      << eggNumberStr << colors::RESET << std::endl;
            return false;
        }
    } else {
        std::cerr << colors::T_RED << "[WARNING] Invalid egg number format: "
                  << eggNumberStr << colors::RESET << std::endl;
        return false;
    }

    if (iss.fail() || prefix != "edi" || eggNumber < 0) {
        std::cerr << colors::T_RED << "[WARNING] Invalid egg death format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updateEggDeath(eggNumber);
    }

    std::cout << colors::T_YELLOW << "[INFO] Egg " << eggNumber
              << " has died."
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleSstMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int timeUnit;

    iss >> prefix >> timeUnit;
    if (iss.fail() || prefix != "sst" || timeUnit <= 0) {
        std::cerr << colors::T_RED
                  << "[WARNING] Invalid time unit modification format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->setTimeUnit(timeUnit, false);
    }

    std::cout << colors::T_YELLOW << "[INFO] Time unit modified to: " << timeUnit
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleSegMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, teamName;

    iss >> prefix >> teamName;
    if (iss.fail() || prefix != "seg") {
        std::cerr << colors::T_RED << "[WARNING] Invalid end of game format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->setGameOver(teamName);
    }
    return true;
}

bool MsgHandler::handleSmgMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, serverMessage;

    iss >> prefix;
    std::getline(iss, serverMessage);

    if (iss.fail() || prefix != "smg") {
        std::cerr << colors::T_RED << "[WARNING] Invalid server message format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->addServerMessage(serverMessage);
    }
    return true;
}

bool MsgHandler::handleSucMessage(const std::string& message)
{
    if (message.empty())
        return false;

    if (message != "suc") {
        std::cerr << colors::T_RED << "[WARNING] Invalid unknown command format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    std::cout << colors::T_RED << "[SERVER] Unknown command received by server"
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleSbpMessage(const std::string& message)
{
    if (message.empty())
        return false;

    if (message != "sbp") {
        std::cerr << colors::T_RED << "[WARNING] Invalid command parameter format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    std::cout << colors::T_RED << "[SERVER] Bad parameters in command"
              << colors::RESET << std::endl;
    return true;
}
