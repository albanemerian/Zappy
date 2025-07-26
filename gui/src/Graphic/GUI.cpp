/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GUI
*/

#include <cmath>
#include <memory>
#include <iostream>
#include <algorithm>
#include <string>
#include <limits>
#include <random>
#include <utility>
#include <chrono>
#include <thread>
#include "GUI.hpp"
#include "../Exceptions/Exceptions.hpp"
#include "../Audio/IAudio.hpp"
#include "../Utils/Constants.hpp"
#include "../Utils/GamepadConstants.hpp"

GUI::GUI(std::shared_ptr<GameInfos> gameInfos,
    const std::string &libGraphic,
    const std::string &libAudioPath
): _isRunning(false), _gameInfos(gameInfos), _backgroundLoaded(false),
        _skyboxLoaded(false), _hoveredPlayerId(-1), _selectedTile({-1, -1})
{
    this->_dlLoaderGraphic = DLLoader<std::shared_ptr<IDisplay>>();
    if (libGraphic.empty())
        throw Exceptions::ModuleGraphicError(libGraphic + ": Lib input empty");
    this->_dlLoaderGraphic.Open(libGraphic.c_str());
    if (!this->_dlLoaderGraphic.getHandler())
        throw Exceptions::ModuleGraphicError(libGraphic + ": Failed to open library: " +
            std::string(dlerror()));

    getTypeFunc_t getTypeFunc = reinterpret_cast<getTypeFunc_t>(
        this->_dlLoaderGraphic.Symbol("getType"));
    if (!getTypeFunc || getTypeFunc() != ModuleType_t::DISPLAY_MODULE)
        throw Exceptions::ModuleGraphicError(libGraphic + ": Not a valid module");

    createGraphicFunc_t createGraphicFunc = reinterpret_cast<createGraphicFunc_t>(
        this->_dlLoaderGraphic.Symbol("create"));
    if (!createGraphicFunc)
        throw Exceptions::ModuleGraphicError(libGraphic +
            ": No create symbole found in the shared lib");
    this->_display = createGraphicFunc();
    std::cout << libGraphic + ": Module GUI Found" << std::endl;


    this->_dlLoaderAudio = DLLoader<std::shared_ptr<IDisplay>>();
    if (libAudioPath.empty())
        throw Exceptions::ModuleAudioError(libAudioPath + ": Lib input empty");
    this->_dlLoaderAudio.Open(libAudioPath.c_str());
    if (!this->_dlLoaderAudio.getHandler())
        throw Exceptions::ModuleAudioError(libAudioPath + ": Failed to open library: " +
            std::string(dlerror()));

    getTypeFunc = reinterpret_cast<getTypeFunc_t>(
        this->_dlLoaderAudio.Symbol("getType"));
    if (!getTypeFunc || getTypeFunc() != ModuleType_t::AUDIO_MODULE)
        throw Exceptions::ModuleAudioError(libAudioPath + ": Not a valid module");

    createAudioFunc_t createAudioFunc = reinterpret_cast<createAudioFunc_t>(
        this->_dlLoaderAudio.Symbol("create"));
    if (!createAudioFunc)
        throw Exceptions::ModuleAudioError(libAudioPath +
            ": No create symbole found in the shared lib");
    this->_audio = createAudioFunc();

    std::cout << libAudioPath + ": Module Audio Found" << std::endl;
    _cameraMode = zappy::gui::CameraMode::FREE;
    auto monitorSize = this->_display->getMonitorSize();
    this->_windowWidth = monitorSize.x;
    this->_windowHeight = monitorSize.y;

    this->_display->initWindow(this->_windowWidth, this->_windowHeight,
        zappy::gui::WINDOW_TITLE);
    this->_display->initCamera();
    _isRunning = this->_display->isWindowReady();
    this->_display->setTargetFPS(zappy::gui::FPS);
    this->_gameInfos->setAudio(this->_audio);

this->_gameInfos->setPerformanceMode(_performanceMode);
    this->_splashScreen = std::make_unique<SplashScreen>(this->_display);
    this->_splashScreen->show();
    this->_splashScreen->setLoadingText("Initializing Graphics...");
    this->_audio->playSound("splash_screen", this->_audio->getSFXVolumeLevel());
    auto startTime = std::chrono::steady_clock::now();
    auto time = this->_getRandomTime(100, 350);
    drawSplashFrame();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::steady_clock::now() - startTime).count() < time){}
    drawSplashFrame();

    this->_splashScreen->setLoadingProgress(0.1f);
    this->_splashScreen->setLoadingText("Loading Resources...");
    startTime = std::chrono::steady_clock::now();
    time = this->_getRandomTime(100, 350);
    drawSplashFrame();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::steady_clock::now() - startTime).count() < time){}
    drawSplashFrame();
    if (!this->_display->loadFont("default", zappy::gui::CUSTOM_FONT_PATH)) {
        std::cout << colors::T_RED << "[WARNING] Failed to load custom font: "
        << zappy::gui::CUSTOM_FONT_PATH << ". Using default font."
        << colors::RESET << std::endl;
    }

    this->_splashScreen->setLoadingProgress(0.2f);
    this->_splashScreen->setLoadingText("Setting up Camera...");
    startTime = std::chrono::steady_clock::now();
    time = this->_getRandomTime(100, 350);
    drawSplashFrame();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::steady_clock::now() - startTime).count() < time){}
    drawSplashFrame();

    _cameraManager = std::make_shared<CameraManager>(this->_display);
    _cameraManager->setGameInfos(_gameInfos);

    this->_splashScreen->setLoadingProgress(0.4f);
    this->_splashScreen->setLoadingText("Creating World...");
    startTime = std::chrono::steady_clock::now();
    time = this->_getRandomTime(100, 350);
    drawSplashFrame();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::steady_clock::now() - startTime).count() < time){}
    drawSplashFrame();

    this->_map = std::make_unique<Map>(_gameInfos, this->_display);
    _cameraManager->setMapInstance(std::shared_ptr<Map>(_map.get(), [](Map*){}));

    this->_splashScreen->setLoadingProgress(0.6f);
    this->_splashScreen->setLoadingText("Positioning Camera...");
    startTime = std::chrono::steady_clock::now();
    time = this->_getRandomTime(100, 350);
    drawSplashFrame();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::steady_clock::now() - startTime).count() < time){}
    drawSplashFrame();

    const auto& mapSize = _gameInfos->getMapSize();

    Vector3f mapCenter = {
        static_cast<float>(mapSize.first - 1) / 2.0f, 0.0f,
        static_cast<float>(mapSize.second - 1) / 2.0f
    };

    float mapScale = std::max(mapSize.first, mapSize.second) * 0.5f;
    mapCenter.y = mapScale * 0.2f;
    _cameraManager->setMapCenter(mapCenter);
    _cameraManager->setMapSize(mapSize.first, mapSize.second);

    this->_splashScreen->setLoadingProgress(0.8f);
    this->_splashScreen->setLoadingText("Creating Interface...");
    startTime = std::chrono::steady_clock::now();
    time = this->_getRandomTime(100, 350);
    drawSplashFrame();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::steady_clock::now() - startTime).count() < time){}

    this->_hud = std::make_unique<HUD>(this->_display, _gameInfos, _audio, _cameraManager,
        _performanceMode,
    [this]() {
        this->switchCameraMode(zappy::gui::CameraMode::FREE);
    });

    this->_splashScreen->setLoadingProgress(0.9f);
    this->_splashScreen->setLoadingText("Loading 3D Models...");
    drawSplashFrame();
    initModels();

    this->_splashScreen->setLoadingProgress(1.0f);
    this->_splashScreen->setLoadingText("Ready!");
    drawSplashFrame();

    startTime = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startTime).count() < 500) {
        drawSplashFrame();
    }
    _loadingComplete = true;
    this->_audio->playMainTheme(this->_audio->getMusicVolumeLevel());
}

GUI::~GUI()
{
    this->_display->closeWindow();
}

void GUI::stop()
{
    this->_isRunning = false;
}

void GUI::run()
{
    if (!_isRunning) {
        return;
    }

    while (_isRunning) {
        update();
        draw();
    }
}

int GUI::_getRandomTime(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

void GUI::updateCamera()
{
    _cameraManager->updateCamera(_cameraMode);
}

void GUI::update()
{
    this->_isRunning = this->_display->isOpen();
    if (_showingSplashScreen) {
        float deltaTime = this->_display->getFrameTime();
        _splashScreen->update(deltaTime);
        if (_loadingComplete) {
            _splashScreen->finish();
            _showingSplashScreen = false;
        }
        return;
    }
    if (_gameInfos->getMapSize().first * _gameInfos->getMapSize().second >= 2500) {
        bool wasPerformanceMode = _performanceMode;
        _performanceMode = true;
        _gameInfos->setPerformanceMode(true);

        if (!wasPerformanceMode && _cameraMode == zappy::gui::CameraMode::TARGETED)
            switchCameraMode(zappy::gui::CameraMode::FREE);
    }

    if (this->_display->isKeyReleased(this->_display->getKeyId(ESC))) {
        if (_selectedTile.first != -1 || _selectedTile.second != -1) {
            this->_selectedTile = {-1, -1};
            _hud->setSelectedTile(-1, -1);
        }
    }

    if (this->_display->isKeyReleased(this->_display->getKeyId(TAB)) ||
        this->_display->isGamepadButtonReleased(this->_display->getKeyId(GM_PD_LEFT_SHOULDER)))
        switchCameraModeNext();

    if (_cameraMode == zappy::gui::CameraMode::PLAYER) {
        if (this->_display->isKeyReleased(this->_display->getKeyId(UP)) ||
            this->_display->isGamepadButtonReleased(this->_display->getKeyId(GM_PD_UP)))
            switchToNextPlayer();
        if (this->_display->isKeyReleased(this->_display->getKeyId(DOWN)) ||
            this->_display->isGamepadButtonReleased(this->_display->getKeyId(GM_PD_DOWN)))
            switchToPreviousPlayer();
    }

    if (this->_display->isKeyReleased(this->_display->getKeyId(H)) ||
        this->_display->isGamepadButtonReleased(this->_display->getKeyId(GM_PD_H)))
        this->_isHUDVisible = !this->_isHUDVisible;

    updateCamera();
    handlePlayerClicks();
    if (this->_isHUDVisible) {
        this->_hud->updateTeamPlayersDisplay(this->_gameInfos);
        this->_hud->updatePlayerInventoryDisplay(this->_cameraManager->getPlayerId(),
            this->_cameraMode);
        this->_hud->updateHelpInformationHUD(this->_cameraMode);
        this->_hud->update();
    }

    this->_audio->playNextTheme(_audio->getMusicVolumeLevel());
}

void GUI::refresh()
{
    update();
}

bool GUI::isRunning()
{
    return this->_isRunning;
}

void GUI::draw()
{
    if (!this->_display->isOpen())
        return;

    if (_showingSplashScreen) {
        _splashScreen->draw();
        return;
    }

    this->_display->beginDrawing();

    this->_display->clearBackground(Color32{255, 255, 255, 255});

    this->_display->begin3DMode();

    if (_skyboxLoaded && !this->_performanceMode)
        this->_display->drawSkybox("skybox");

    if (!this->_performanceMode) {
        const auto& mapSize = _gameInfos->getMapSize();
        float forestX = static_cast<float>(mapSize.first) * 2.0f + 20.0f;
        float forestZ = static_cast<float>(mapSize.second) * 2.0f + 20.0f;

        this->_display->drawModelEx(
            "forest", {forestX, -5.0f, forestZ}, {0.0f, 90.0f, 0.0f},
            -135.0f, {1.0f, 1.0f, 1.0f}, CWHITE);
    }

    _map->draw(_performanceMode);

    if (_hoveredPlayerId >= 0) {
        const auto& players = _gameInfos->getPlayers();
        auto playerIt = std::find_if(players.begin(), players.end(),
            [this](const zappy::structs::Player& player) {
                return player.number == _hoveredPlayerId;
            });

        if (playerIt != players.end()) {
            BoundingBox3D playerBox = getPlayerBoundingBox(*playerIt);
            Vector3f center = {
                (playerBox.min.x + playerBox.max.x) / 2.0f,
                (playerBox.min.y + playerBox.max.y) / 2.0f,
                (playerBox.min.z + playerBox.max.z) / 2.0f
            };
            Vector3f size = {
                playerBox.max.x - playerBox.min.x,
                playerBox.max.y - playerBox.min.y,
                playerBox.max.z - playerBox.min.z
            };
            this->_display->drawCubeWires(center, size.x, size.y, size.z, CYELLOW);
        }
    }

    if (_selectedTile.first >= 0 && _selectedTile.second >= 0) {
        BoundingBox3D tileBox = getTileBoundingBox(_selectedTile.first, _selectedTile.second);
        Vector3f center = {
            (tileBox.min.x + tileBox.max.x) / 2.0f,
            (tileBox.min.y + tileBox.max.y) / 2.0f - 0.2f,
            (tileBox.min.z + tileBox.max.z) / 2.0f
        };
        Vector3f size = {
            tileBox.max.x - tileBox.min.x,
            tileBox.max.y - tileBox.min.y,
            tileBox.max.z - tileBox.min.z
        };
        this->_display->drawCubeWires(center, size.x, size.y, size.z, CYELLOW);
    }

    this->_display->end3DMode();
    if (this->_isHUDVisible) {
        this->_hud->draw();
    }
    this->_display->endDrawing();
}

int GUI::getWindowWidth() const
{
    return _windowWidth;
}
int GUI::getWindowHeight() const
{
    return _windowHeight;
}
void GUI::setWindowWidth(int width)
{
    if (width <= 0 || width > this->_display->getMonitorSize().x)
        return;

    _windowWidth = width;
    auto screenSize = this->_display->getScreenSize();
    this->_display->initWindow(_windowWidth, _windowHeight, zappy::gui::WINDOW_TITLE);
    _hud->handleResize(screenSize.x, screenSize.y,
        _windowWidth, _windowHeight);
}

void GUI::setWindowHeight(int height)
{
    if (height <= 0 || height > this->_display->getMonitorSize().y)
        return;

    _windowHeight = height;
    auto screenSize = this->_display->getScreenSize();
    this->_display->initWindow(_windowWidth, _windowHeight, zappy::gui::WINDOW_TITLE);
    _hud->handleResize(screenSize.x, screenSize.y,
        _windowWidth, _windowHeight);
}

void GUI::switchCameraMode(zappy::gui::CameraMode mode)
{
    if (_performanceMode && mode == zappy::gui::CameraMode::TARGETED)
        return;

    if (mode == zappy::gui::CameraMode::TARGETED &&
        _cameraMode != zappy::gui::CameraMode::TARGETED) {
        const auto& mapSize = _gameInfos->getMapSize();

        Vector3f mapCenter = {
            static_cast<float>(mapSize.first - 1) / 2.0f, 0.0f,
            static_cast<float>(mapSize.second - 1) / 2.0f
        };

        float mapScale = std::max(mapSize.first, mapSize.second) * 0.5f;
        mapCenter.y = mapScale * 0.2f;

        _cameraManager->setMapCenter(mapCenter);
        _cameraManager->initTargetPositionFromCurrentCamera();
    }

    if (mode == zappy::gui::CameraMode::PLAYER) {
        if (_cameraManager->getPlayerId() < 0 ||
            !playerExists(_cameraManager->getPlayerId())) {
            if (!selectFirstAvailablePlayer()) {
                mode = static_cast<zappy::gui::CameraMode>(
                    (static_cast<int>(mode) + 1) %
                        static_cast<int>(zappy::gui::CameraMode::NB_MODES));
            }
        }
    }

    if (mode == zappy::gui::CameraMode::PLAYER && _cameraMode !=
        zappy::gui::CameraMode::PLAYER) {
        _hud->initPlayerInventoryDisplay(_cameraManager->getPlayerId());
    } else if (mode != zappy::gui::CameraMode::PLAYER && _cameraMode ==
        zappy::gui::CameraMode::PLAYER) {
        _hud->clearPlayerInventoryElements();
    }

    _cameraMode = mode;
    _gameInfos->setCurrentCameraMode(_cameraMode);
}

void GUI::switchCameraModeNext()
{
    zappy::gui::CameraMode newMode = static_cast<zappy::gui::CameraMode>(
        (static_cast<int>(_cameraMode) + 1) %
            static_cast<int>(zappy::gui::CameraMode::NB_MODES));

    if (_performanceMode && newMode == zappy::gui::CameraMode::TARGETED) {
        newMode = static_cast<zappy::gui::CameraMode>(
            (static_cast<int>(newMode) + 1) %
                static_cast<int>(zappy::gui::CameraMode::NB_MODES));
    }

    switchCameraMode(newMode);
}

void GUI::setPlayerToFollow(int playerId)
{
    _cameraManager->setPlayerId(playerId);
    _gameInfos->setCurrentPlayerFocus(playerId);
    if (_cameraMode == zappy::gui::CameraMode::PLAYER) {
        _hud->initPlayerInventoryDisplay(playerId);
    }
}

int GUI::getPlayerToFollow() const
{
    return _cameraManager->getPlayerId();
}

bool GUI::selectFirstAvailablePlayer()
{
    const auto& players = _gameInfos->getPlayers();

    if (players.empty())
        return false;

    for (const auto& player : players) {
        if (_gameInfos->isTeamVisible(player.teamName)) {
            _cameraManager->setPlayerId(player.number);
            return true;
        }
    }

    return false;
}

bool GUI::playerExists(int playerId) const
{
    const auto& players = _gameInfos->getPlayers();

    auto playerIt = std::find_if(players.begin(), players.end(),
        [playerId](const zappy::structs::Player& player) {
            return player.number == playerId;
        });

    if (playerIt != players.end())
        return _gameInfos->isTeamVisible(playerIt->teamName);

    return false;
}

void GUI::switchToNextPlayer()
{
    const auto& players = _gameInfos->getPlayers();

    if (players.empty())
        return;

    int currentPlayerId = _cameraManager->getPlayerId();
    bool foundCurrent = false;
    int nextPlayerId = -1;

    for (const auto& player : players) {
        if (foundCurrent && _gameInfos->isTeamVisible(player.teamName)) {
            nextPlayerId = player.number;
            break;
        }
        if (player.number == currentPlayerId) {
            foundCurrent = true;
        }
    }

    if (nextPlayerId == -1) {
        for (const auto& player : players) {
            if (_gameInfos->isTeamVisible(player.teamName)) {
                nextPlayerId = player.number;
                break;
            }
        }
    }

    if (nextPlayerId != -1) {
        _cameraManager->setPlayerId(nextPlayerId);
    }
}

void GUI::switchToPreviousPlayer()
{
    const auto& players = _gameInfos->getPlayers();

    if (players.empty())
        return;

    int currentPlayerId = _cameraManager->getPlayerId();
    int prevPlayerId = -1;

    for (const auto& player : players) {
        if (player.number == currentPlayerId)
            break;

        if (_gameInfos->isTeamVisible(player.teamName))
            prevPlayerId = player.number;
    }

    if (prevPlayerId == -1) {
        for (auto it = players.rbegin(); it != players.rend(); ++it) {
            if (_gameInfos->isTeamVisible(it->teamName)) {
                prevPlayerId = it->number;
                break;
            }
        }
    }

    if (prevPlayerId != -1)
        _cameraManager->setPlayerId(prevPlayerId);
}

void GUI::initModels()
{
    if (this->_display->loadSkybox("skybox", "gui/assets/sprite/skybox/skybox.jpg")) {
        std::cout << colors::T_GREEN << "[INFO] Successfully loaded skybox texture."
                << colors::RESET << std::endl;
        _skyboxLoaded = true;
    } else {
        std::cout << colors::T_RED << "[ERROR] Failed to load skybox texture."
                << colors::RESET << std::endl;
        if (!_backgroundLoaded) {
            if (!this->_display->loadTexture("background", "gui/assets/sprite/background.png"))
                std::cout << colors::T_RED << "[ERROR] Failed to load background texture."
                        << colors::RESET << std::endl;
            else
                _backgroundLoaded = true;
        }
    }

    if (!this->_display->loadModel("forest", "gui/assets/models/forest.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load forest model."
                << colors::RESET << std::endl;
    else
        std::cout << colors::T_GREEN << "[INFO] Successfully loaded forest model."
                << colors::RESET << std::endl;

    initPlayers();

    if (!this->_display->loadModel("platform", "gui/assets/models/tile.glb",
        {0.0f, 0.25f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load platform model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("food", "gui/assets/models/apple.glb", {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load food model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("linemate", "gui/assets/models/soccerball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load linemate model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("deraumere", "gui/assets/models/beachball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load deraumere model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("sibur", "gui/assets/models/basketball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load sibur model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("mendiane", "gui/assets/models/bowlingball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load mendiane model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("phiras", "gui/assets/models/eightball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load phiras model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("thystame", "gui/assets/models/tennisball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load thystame model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("egg", "gui/assets/models/egg.glb", {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load egg model."
                  << colors::RESET << std::endl;
}

void GUI::initPlayers()
{
    for (auto &playerModelInfo : zappy::gui::PLAYER_MODELS_INFO) {
        if (!this->_display->loadModel(playerModelInfo.name,
            playerModelInfo.modelPath, playerModelInfo.center))
            std::cout << colors::T_RED << "[ERROR] Failed to load model: "
                      << playerModelInfo.name << colors::RESET << std::endl;
    }
}

void GUI::handlePlayerClicks()
{
    std::lock_guard<std::mutex> lock(_playerMutex);

    _hoveredPlayerId = getPlayerUnderMouse();

    if (this->_display->isMouseButtonPressed(this->_display->getKeyId(MOUSE_LEFT))) {
        if (_hud && _hud->isMouseOverHUD())
            return;

        if (_hoveredPlayerId >= 0) {
            setPlayerToFollow(_hoveredPlayerId);
            switchCameraMode(zappy::gui::CameraMode::PLAYER);
            _audio->playSound("clickPlayer", this->_audio->getSFXVolumeLevel());
        } else {
            handleTileClicks();
        }
    }
}

int GUI::getPlayerUnderMouse() const
{
    Vector2f mousePos = this->_display->getMousePosition();
    Ray3D mouseRay = this->_display->getMouseRay(mousePos);

    const auto& players = _gameInfos->getPlayers();
    float closestDistance = std::numeric_limits<float>::max();
    int closestPlayerId = -1;

    for (const auto& player : players) {
        if (!_gameInfos->isTeamVisible(player.teamName))
            continue;

        if (_performanceMode) {
            Vector3f playerPos =
                _map->getPlayerInterpolatedPosition(player.number, player.x, player.y);
            float distanceFromCamera = _display->vector3DDistanceFromCamera(playerPos);
            if (distanceFromCamera > zappy::gui::FOG_DISTANCE_MAX) {
                continue;
            }
        }

        BoundingBox3D playerBox = getPlayerBoundingBox(player);
        RayCollision3D collision = this->_display->getRayCollisionBox(mouseRay, playerBox);

        if (collision.hit && collision.distance < closestDistance) {
            closestDistance = collision.distance;
            closestPlayerId = player.number;
        }
    }
    return closestPlayerId;
}

BoundingBox3D GUI::getPlayerBoundingBox(const zappy::structs::Player& player) const
{
    Vector3f playerPos = _map->getPlayerInterpolatedPosition(player.number, player.x,
        player.y);

    size_t stackIndex = 0;
    const auto& allPlayers = _gameInfos->getPlayers();
    for (const auto& otherPlayer : allPlayers) {
        if (otherPlayer.number != player.number &&
            otherPlayer.x == player.x && otherPlayer.y == player.y &&
            otherPlayer.number < player.number) {
            stackIndex++;
        }
    }

    playerPos.y = _map->getOffset(DisplayPriority::PLAYER, player.x, player.y,
        stackIndex) + 0.3f;

    const float boxWidth = 1.0f;
    const float boxHeight = 1.0f;
    const float boxDepth = 1.0f;

    Vector3f min = {
        playerPos.x - boxWidth / 2.0f,
        playerPos.y - boxHeight / 2.0f,
        playerPos.z - boxDepth / 2.0f
    };

    Vector3f max = {
        playerPos.x + boxWidth / 2.0f,
        playerPos.y + boxHeight / 2.0f,
        playerPos.z + boxDepth / 2.0f
    };

    return {min, max};
}

void GUI::handleVictory(const std::string &teamName)
{
    if (_hud) {
        _hud->displayWinMessage(teamName);
    }
}

void GUI::handleTileClicks()
{
    std::pair<int, int> hoveredTile = getTileUnderMouse();

    if (hoveredTile.first >= 0 && hoveredTile.second >= 0) {
        std::pair<int, int> previousSelectedTile = _selectedTile;

        if (_selectedTile.first == hoveredTile.first &&
            _selectedTile.second == hoveredTile.second)
            _selectedTile = {-1, -1};
        else
            _selectedTile = hoveredTile;

        if (_selectedTile != previousSelectedTile)
            _hud->setSelectedTile(_selectedTile.first, _selectedTile.second);

        _audio->playSound("click", 100.0f);
    }
}

std::pair<int, int> GUI::getTileUnderMouse() const
{
    Vector2f mousePos = this->_display->getMousePosition();
    Ray3D mouseRay = this->_display->getMouseRay(mousePos);

    auto mapSize = _gameInfos->getMapSize();
    int mapWidth = mapSize.first;
    int mapHeight = mapSize.second;

    float closestDistance = std::numeric_limits<float>::max();
    std::pair<int, int> closestTile = {-1, -1};

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (_performanceMode) {
                Vector3f tilePosition = {
                    static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
                    0.0f,
                    static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
                };
                float distanceFromCamera = _display->vector3DDistanceFromCamera(tilePosition);
                if (distanceFromCamera > zappy::gui::FOG_DISTANCE_MAX) {
                    continue;
                }
            }

            BoundingBox3D tileBox = getTileBoundingBox(x, y);
            RayCollision3D collision = this->_display->getRayCollisionBox(mouseRay, tileBox);

            if (collision.hit && collision.distance < closestDistance) {
                closestDistance = collision.distance;
                closestTile = {x, y};
            }
        }
    }
    return closestTile;
}

BoundingBox3D GUI::getTileBoundingBox(int x, int y) const
{
    Vector3f tilePosition = {
        static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
        0.0f,
        static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
    };

    const float tileSize = 2.0f;
    const float tileHeight = 0.4f;

    Vector3f min = {
        tilePosition.x - tileSize / 2.0f,
        tilePosition.y - tileHeight / 2.0f,
        tilePosition.z - tileSize / 2.0f
    };

    Vector3f max = {
        tilePosition.x + tileSize / 2.0f,
        tilePosition.y + tileHeight / 2.0f,
        tilePosition.z + tileSize / 2.0f
    };

    return {min, max};
}

void GUI::drawSplashFrame()
{
    if (!this->_display->isOpen() || !this->_splashScreen) {
        return;
    }
    float deltaTime = this->_display->getFrameTime();
    this->_splashScreen->update(deltaTime);
    this->_splashScreen->draw();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
}
