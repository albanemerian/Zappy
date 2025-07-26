/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HUD
*/

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <cmath>
#include "../../Utils/Constants.hpp"
#include "../../Utils/InputType.hpp"
#include "HUD.hpp"

HUD::HUD(std::shared_ptr<IDisplay> display, std::shared_ptr<GameInfos> gameInfos,
         std::shared_ptr<IAudio> audio,
         std::shared_ptr<CameraManager> camera,
         bool performanceMode,
         std::function<void()> resetCameraFunc)
    : _containers(), _display(display), _gameInfos(gameInfos), _audio(audio), _camera(camera),
      _performanceMode(performanceMode),
      _resetCameraFunc(resetCameraFunc),
      _showVictoryMessage(false),
      _winningTeam(""),
      _victoryColor({0, 127, 255, 255}),
      _selectedTile(-1, -1),
      _hoveredTeam(""),
      _teamDetailsContainer(nullptr),
      _mapInfoButtonHovered(false)
{
    _help = std::make_shared<Help>(display, audio);
    _settings = std::make_shared<Settings>(display, audio, camera, gameInfos);
    initDefaultLayout(15.0f, 20.0f);
    initExitButton();
    initSettingsButton();
    initHelpButton();
    initCameraResetButton();
    initTeamPlayersDisplay(_gameInfos);
    initTpsSlider(_gameInfos, _display, _audio);
    initServerMessagesDisplay(_gameInfos);
    initMapInfoDisplay();
    initMapInfoButton();
    this->_initHelpInformation();

    if (_gameInfos) {
        _gameInfos->addObserver(std::shared_ptr<IObserver>(this, [](IObserver*) {}));
    }
}

HUD::~HUD()
{
    _containers.clear();
}

void HUD::draw()
{
    for (const auto &pair : _containers) {
        if (pair.first != "message_container" && pair.first != "map_info_container") {
            pair.second->draw();
        }
    }

    if (_help && _help->isVisible()) {
        _help->draw();
    }
    if (this->_settings && this->_settings->isVisible()) {
        this->_settings->draw();
    }

    auto mapInfoContainer = _containers.find("map_info_container");
    if (mapInfoContainer != _containers.end() && mapInfoContainer->second->isVisible())
        mapInfoContainer->second->draw();

    auto msgContainer = _containers.find("message_container");
    if (msgContainer != _containers.end() && msgContainer->second->isVisible())
        msgContainer->second->draw();

    if (_teamDetailsContainer && _teamDetailsContainer->isVisible())
        _teamDetailsContainer->draw();

    if (_showVictoryMessage && !_winningTeam.empty()) {
        std::string message = "Team " + _winningTeam + " WINS!";

        Vector2i screenDimensions = _display->getScreenSize();
        int screenWidth = screenDimensions.x;
        int screenHeight = screenDimensions.y;

        float fontSize = 60.0f;
        float outlineThickness = 2.0f;

        float textWidth = _display->measureText(message, fontSize);
        float textX = (screenWidth - textWidth) / 2.0f;
        float textY = screenHeight / 8.0f - fontSize / 2.0f;

        Color32 outlineColor = {0, 0, 0, 255};

        std::vector<std::pair<float, float>> offsets = {
            {-outlineThickness, -outlineThickness},
            {-outlineThickness, 0},
            {-outlineThickness, outlineThickness},
            {0, -outlineThickness},
            {0, outlineThickness},
            {outlineThickness, -outlineThickness},
            {outlineThickness, 0},
            {outlineThickness, outlineThickness}
        };

        for (const auto& offset : offsets) {
            _display->drawText(message, textX + offset.first, textY + offset.second,
                fontSize, outlineColor);
        }
        _display->drawText(
            message,
            textX,
            textY,
            fontSize,
            _victoryColor
        );
    }
}

void HUD::update()
{
    for (auto& pair : _containers) {
        pair.second->update();
    }

    if (_help) {
        _help->update();
    }

    if (this->_settings) {
        this->_settings->update();
    }

    updateTeamPlayersDisplay(_gameInfos);
    updateTpsSlider(_gameInfos);
    updateServerMessagesDisplay(_gameInfos);
    updateMapInfoDisplay();
    updateFpsDisplay();
    updateTeamHoverDetection();

    if (_selectedTile.first >= 0 && _selectedTile.second >= 0)
        updateTileResourceDisplay(_selectedTile.first, _selectedTile.second);
}

std::shared_ptr<Containers> HUD::addContainer(
    const std::string& id,
    float x, float y,
    float width, float height,
    Color32 backgroundColor
)
{
    if (_containers.find(id) != _containers.end())
        return nullptr;

    auto container = std::make_shared<Containers>(
        _display,
        _audio,
        x,
        y,
        width,
        height,
        backgroundColor);

    _containers[id] = container;

    return container;
}

std::shared_ptr<Containers> HUD::getContainer(const std::string& id) const
{
    auto it = _containers.find(id);

    if (it != _containers.end())
        return it->second;

    return nullptr;
}

bool HUD::removeContainer(const std::string& id)
{
    auto it = _containers.find(id);

    if (it != _containers.end()) {
        _containers.erase(it);
        return true;
    }
    return false;
}

void HUD::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    for (auto& pair : _containers) {
        pair.second->handleResize(oldWidth, oldHeight, newWidth, newHeight);
    }

    if (_help)
        _help->handleResize(oldWidth, oldHeight, newWidth, newHeight);

    if (this->_settings)
        this->_settings->handleResize(oldWidth, oldHeight, newWidth, newHeight);

    if (_teamDetailsContainer)
        _teamDetailsContainer->handleResize(oldWidth, oldHeight, newWidth, newHeight);
}

void HUD::clearAllContainers()
{
    _containers.clear();
}

void HUD::initDefaultLayout(float sideWidthPercent, float bottomHeightPercent)
{
    if (sideWidthPercent <= 0.0f)
        sideWidthPercent = 15.0f;
    if (bottomHeightPercent <= 0.0f)
        bottomHeightPercent = 20.0f;

    auto screenSize = this->_display->getScreenSize();
    int screenHeight = screenSize.x;
    int screenWidth = screenSize.y;

    float sideWidth = (screenWidth * sideWidthPercent) / 100.0f;
    float bottomHeight = (screenHeight * bottomHeightPercent) / 100.0f;
    float squareSize = sideWidth;

    auto squareContainer = createSquareContainer(squareSize, sideWidthPercent);

    float sideYStart = squareSize;
    float sideHeight = screenHeight - squareSize - bottomHeight;

    auto sideContainer = createSideContainer(
                            sideYStart,
                            sideWidth,
                            sideHeight,
                            sideWidthPercent,
                            bottomHeightPercent);

    auto bottomContainer = createBottomContainer(
                            screenWidth,
                            screenHeight,
                            bottomHeight,
                            bottomHeightPercent);

    auto tpsContainer = createTpsContainer(
                            screenWidth,
                            screenHeight,
                            bottomHeight,
                            bottomHeightPercent);

    auto securityContainer = createSecurityContainer(
                            screenWidth,
                            screenHeight,
                            bottomHeight,
                            bottomHeightPercent);

    auto serverMessagesContainer = createServerMessagesContainer(
                            screenWidth,
                            screenHeight,
                            bottomHeight,
                            bottomHeightPercent);

    auto mapInfoContainer = createMapInfoContainer();

    initFpsDisplay();
}

void HUD::_initHelpInformation()
{
    auto bottomContainer = this->getBottomContainer();

    bottomContainer->addTextPercent(
        "cam_info_mode",
        1.f, 10.f,
        "Camera mod: NONE",
        7.0f,
        {245, 224, 80, 255}
    );

    bottomContainer->addTextPercent(
        "help_cam_key",
        1.f, 30.f,
        "NONE",
        7.0f,
        {255, 236, 183, 255}
    );
}

std::shared_ptr<Containers> HUD::getSideContainer() const
{
    return getContainer("side_container");
}

std::shared_ptr<Containers> HUD::getBottomContainer() const
{
    return getContainer("bottom_container");
}

std::shared_ptr<Containers> HUD::getSquareContainer() const
{
    return getContainer("square_container");
}

std::shared_ptr<Containers> HUD::getTpsContainer() const
{
    return getContainer("tps_container");
}

std::shared_ptr<Containers> HUD::getSecurityContainer() const
{
    return getContainer("security_container");
}

std::shared_ptr<Containers> HUD::getServerMessagesContainer() const
{
    return getContainer("server_messages_container");
}

std::shared_ptr<Containers> HUD::getMapInfoContainer() const
{
    return getContainer("map_info_container");
}

void HUD::initExitButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    squareContainer->addButtonPercent(
        "exit_button",
        15.0f, 10.0f,
        70.0f, 15.0f,
        "EXIT",
        [this]() {
            this->_display->closeWindow();
        },
        {240, 60, 60, 255},
        {255, 100, 100, 255},
        {200, 40, 40, 255},
        {255, 255, 255, 255}
    );
}

void HUD::initSettingsButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    squareContainer->addButtonPercent(
        "settings_button",
        15.0f, 30.0f,
        70.0f, 15.0f,
        "SETTINGS",
        [this]() {
            if (this->_settings) {
                if (this->_settings->isVisible()) {
                    this->_settings->hide();
                } else {
                    if (this->_help && this->_help->isVisible())
                        this->_help->hide();
                    this->_settings->show();
                }
            }
        },
        {60, 60, 240, 255},
        {100, 100, 255, 255},
        {40, 40, 200, 255},
        {255, 255, 255, 255}
    );
}

void HUD::initHelpButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    squareContainer->addButtonPercent(
        "help_button",
        15.0f, 50.0f,
        70.0f, 15.0f,
        "HELP",
        [this]() {
            if (this->_help) {
                if (this->_help->isVisible()) {
                    this->_help->hide();
                } else {
                    if (this->_settings && this->_settings->isVisible())
                        this->_settings->hide();
                    this->_help->show();
                }
            }
        },
        {60, 240, 60, 255},
        {100, 255, 100, 255},
        {40, 200, 40, 255},
        {255, 255, 255, 255}
    );
}

void HUD::initCameraResetButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    squareContainer->addButtonPercent(
        "camera_reset_button",
        15.0f, 70.0f,
        70.0f, 15.0f,
        "RESET CAMERA",
        [this]() {
            this->_display->initCamera();

            if (_resetCameraFunc) {
                _resetCameraFunc();
            }
        },
        {245, 224, 80, 255},
        {255, 235, 120, 255},
        {200, 200, 40, 255},
        {255, 255, 255, 255}
    );
}

void HUD::initTeamPlayersDisplay(std::shared_ptr<GameInfos> gameInfos)
{
    auto sideContainer = getSideContainer();
    if (!sideContainer || !gameInfos)
        return;

    clearTeamDisplayElements(sideContainer);

    const std::vector<std::string> teams = gameInfos->getTeamNames();
    const std::vector<zappy::structs::Player> players = gameInfos->getPlayers();

    float yPos = 5.0f;

    for (int i = static_cast<int>(teams.size()) - 1; i >= 0; i--) {
        std::string teamId = "team_display_" + std::to_string(i);
        const std::string& teamName = teams[i];

        if (i < static_cast<int>(teams.size()) - 1) {
            sideContainer->addTextPercent(
                teamId + "_separator",
                5.0f,
                yPos - 0.5f,
                std::string(90, '-'),
                1.0f,
                {120, 120, 120, 150}
            );
            yPos += 1.0f;
        }

        sideContainer->addCheckboxPercent(
            teamId + "_checkbox",
            2.0f,
            yPos + 0.2f,
            8.0f,
            3.0f,
            true,
            [this, teamName](bool checked) {
                this->_gameInfos->setTeamVisibility(teamName, checked);
            }
        );

        Color32 teamColor = gameInfos->getTeamColor(teamName);
        sideContainer->addTextPercent(
            teamId + "_title",
            12.0f,
            yPos,
            "TEAM: " + teamName,
            3.5f,
            teamColor
        );

        yPos += 4.0f;

        std::vector<int> teamPlayerNumbers = getTeamPlayerNumbers(teamName, players);
        addPlayerListText(sideContainer, teamId, yPos, teamPlayerNumbers);

        yPos += 3.0f;
        yPos += 2.0f;
    }
}

void HUD::updateTeamPlayersDisplay(std::shared_ptr<GameInfos> gameInfos)
{
    auto sideContainer = getSideContainer();
    if (!sideContainer || !gameInfos)
        return;

    static std::vector<std::string> lastTeamNames;
    const std::vector<std::string> currentTeamNames = gameInfos->getTeamNames();

    if (lastTeamNames != currentTeamNames) {
        lastTeamNames = currentTeamNames;
        initTeamPlayersDisplay(gameInfos);
        return;
    }

    const std::vector<zappy::structs::Player> players = gameInfos->getPlayers();

    for (int i = static_cast<int>(currentTeamNames.size()) - 1; i >= 0; i--) {
        std::string teamId = "team_display_" + std::to_string(i);
        const std::string& teamName = currentTeamNames[i];

        auto titleElem = std::dynamic_pointer_cast<Text>(
            sideContainer->getElement(teamId + "_title"));

        if (titleElem) {
            Color32 teamColor = gameInfos->getTeamColor(teamName);
            titleElem->setColor(teamColor);

            float yPos = titleElem->getBounds().y - sideContainer->getBounds().y;
            yPos = yPos / sideContainer->getBounds().height * 100.0f;
            yPos += 4.0f;

            for (int j = 0; j < 50; j++) {
                sideContainer->removeElement(teamId + "_player_" + std::to_string(j));
            }

            std::vector<int> teamPlayerNumbers = getTeamPlayerNumbers(teamName, players);
            addPlayerListText(sideContainer, teamId, yPos, teamPlayerNumbers);
        }
    }
}

void HUD::clearTeamDisplayElements(std::shared_ptr<Containers> container)
{
    if (!container)
        return;

    for (int i = 0; i < 100; i++) {
        std::string idBase = "team_display_" + std::to_string(i);
        container->removeElement(idBase + "_title");
        container->removeElement(idBase + "_separator");
        container->removeElement(idBase + "_stats");
        container->removeElement(idBase + "_checkbox");

        for (int j = 0; j < 50; j++) {
            container->removeElement(idBase + "_player_" + std::to_string(j));
        }
    }
}

void HUD::clearPlayerInventoryElements()
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    std::vector<std::string> elementIds = {
        "player_info_title",
        "player_info_separator",
        "player_info_level", "player_info_team",
        "player_info_id", "player_info_ritual",
        "player_info_position", "player_info_orientation",
        "player_level_increment_btn", "player_level_decrement_btn",
        "inventory_title",
        "inventory_separator",
        "inventory_food", "inventory_linemate", "inventory_deraumere", "inventory_sibur",
        "inventory_mendiane", "inventory_phiras", "inventory_thystame",
        "inventory_food_button_increment", "inventory_food_button_decrement",
        "inventory_linemate_button_increment", "inventory_linemate_button_decrement",
        "inventory_deraumere_button_increment", "inventory_deraumere_button_decrement",
        "inventory_sibur_button_increment", "inventory_sibur_button_decrement",
        "inventory_mendiane_button_increment", "inventory_mendiane_button_decrement",
        "inventory_phiras_button_increment", "inventory_phiras_button_decrement",
        "inventory_thystame_button_increment", "inventory_thystame_button_decrement",
        "kill_player_button"
    };

    for (const auto& id : elementIds) {
        bottomContainer->removeElement(id);
    }
}

void HUD::addIncrementDecrementButtons(std::shared_ptr<Containers> container, int playerId)
{
    if (!container)
        return;

    Color32 normalColor = {180, 180, 180, 255};
    Color32 hoverColor = {220, 220, 220, 255};
    Color32 pressedColor = {150, 150, 150, 255};
    Color32 textColor = {30, 30, 30, 255};

    container->addButtonPercent("player_level_increment_btn", 56.0f, 53.5f, 1.5f, 10.0f, "+",
        [this, playerId]() {
            this->_gameInfos->incrementPlayerLevel(playerId);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "player_level_decrement_btn", 58.0f, 53.5f, 1.5f, 10.0f, "-",
        [this, playerId]() {
            this->_gameInfos->decrementPlayerLevel(playerId);
        }, normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_food_button_increment", 79.8f, 16.5f, 1.0f, 6.5f, "+",
        [this, playerId]() {
            this->_gameInfos->incrementPlayerInventoryItem(playerId, 0);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_food_button_decrement", 80.9f, 16.5f, 1.0f, 6.5f, "-",
        [this, playerId]() {
            this->_gameInfos->decrementPlayerInventoryItem(playerId, 0);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_linemate_button_increment", 72.8f, 27.5f, 1.0f, 6.5f, "+",
        [this, playerId]() {
            this->_gameInfos->incrementPlayerInventoryItem(playerId, 1);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_linemate_button_decrement", 73.9f, 27.5f, 1.0f, 6.5f, "-",
        [this, playerId]() {
            this->_gameInfos->decrementPlayerInventoryItem(playerId, 1);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_deraumere_button_increment", 72.8f, 40.5f, 1.0f, 6.5f, "+",
        [this, playerId]() {
            this->_gameInfos->incrementPlayerInventoryItem(playerId, 2);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_deraumere_button_decrement", 73.9f, 40.5f, 1.0f, 6.5f, "-",
        [this, playerId]() {
            this->_gameInfos->decrementPlayerInventoryItem(playerId, 2);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_sibur_button_increment", 72.8f, 53.5f, 1.0f, 6.5f, "+",
        [this, playerId]() {
            this->_gameInfos->incrementPlayerInventoryItem(playerId, 3);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_sibur_button_decrement", 73.9f, 53.5f, 1.0f, 6.5f, "-",
        [this, playerId]() {
            this->_gameInfos->decrementPlayerInventoryItem(playerId, 3);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_mendiane_button_increment", 85.8f, 27.5f, 1.0f, 6.5f, "+",
        [this, playerId]() {
            this->_gameInfos->incrementPlayerInventoryItem(playerId, 4);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_mendiane_button_decrement", 86.9f, 27.5f, 1.0f, 6.5f, "-",
        [this, playerId]() {
            this->_gameInfos->decrementPlayerInventoryItem(playerId, 4);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_phiras_button_increment", 85.8f, 40.5f, 1.0f, 6.5f, "+",
        [this, playerId]() {
            this->_gameInfos->incrementPlayerInventoryItem(playerId, 5);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_phiras_button_decrement", 86.9f, 40.5f, 1.0f, 6.5f, "-",
        [this, playerId]() {
            this->_gameInfos->decrementPlayerInventoryItem(playerId, 5);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_thystame_button_increment", 85.8f, 53.5f, 1.0f, 6.5f, "+",
        [this, playerId]() {
            this->_gameInfos->incrementPlayerInventoryItem(playerId, 6);
        },
        normalColor, hoverColor, pressedColor, textColor
    );

    container->addButtonPercent(
        "inventory_thystame_button_decrement", 86.9f, 53.5f, 1.0f, 6.5f, "-",
        [this, playerId]() {
            this->_gameInfos->decrementPlayerInventoryItem(playerId, 6);
        },
        normalColor, hoverColor, pressedColor, textColor
    );
}

std::vector<int> HUD::getTeamPlayerNumbers(
    const std::string& teamName,
    const std::vector<zappy::structs::Player>& players)
{
    std::vector<int> teamPlayerNumbers;
    for (const auto& player : players) {
        if (player.teamName == teamName)
            teamPlayerNumbers.push_back(player.number);
    }
    return teamPlayerNumbers;
}

std::string HUD::createPlayerListText(const std::vector<int>& playerNumbers)
{
    if (playerNumbers.empty())
        return "No players";

    std::string playerList = "Players: " + std::to_string(playerNumbers.size())
    + " alive";
    return playerList;
}

void HUD::addPlayerListText(
    std::shared_ptr<Containers> container,
    const std::string& teamId,
    float yPos,
    const std::vector<int>& playerNumbers)
{
    if (!container)
        return;

    std::string playerList = createPlayerListText(playerNumbers);

    if (!playerNumbers.empty()) {
        container->addTextPercent(
            teamId + "_player_0",
            14.0f,
            yPos,
            playerList,
            2.2f,
            {200, 200, 200, 255}
        );
    } else {
        container->addTextPercent(
            teamId + "_player_0",
            14.0f,
            yPos,
            playerList,
            2.0f,
            {150, 150, 150, 200}
        );
    }
}

std::shared_ptr<Containers> HUD::createSquareContainer(
    float squareSize,
    float sideWidthPercent)
{
    auto squareContainer = addContainer(
        "square_container",
        0, 0,
        squareSize, squareSize,
        {60, 60, 60, 220}
    );

    if (squareContainer)
        squareContainer->setRelativePosition(0, 0, sideWidthPercent, sideWidthPercent);

    return squareContainer;
}

std::shared_ptr<Containers> HUD::createSideContainer(
    float sideYStart,
    float sideWidth,
    float sideHeight,
    float sideWidthPercent,
    float bottomHeightPercent)
{
    auto sideContainer = addContainer(
        "side_container",
        0, sideYStart,
        sideWidth, sideHeight,
        {40, 40, 40, 200}
    );

    if (sideContainer) {
        sideContainer->setRelativePosition(
            0,
            sideWidthPercent,
            sideWidthPercent,
            100.0f - sideWidthPercent - bottomHeightPercent);
    }

    return sideContainer;
}

std::shared_ptr<Containers> HUD::createBottomContainer(
    int screenWidth,
    int screenHeight,
    float bottomHeight,
    float bottomHeightPercent)
{
    auto bottomContainer = addContainer(
        "bottom_container",
        0, screenHeight - bottomHeight,
        screenWidth, bottomHeight,
        {40, 40, 40, 200}
    );

    if (bottomContainer) {
        bottomContainer->setRelativePosition(
            0,
            100.0f - bottomHeightPercent,
            100.0f, bottomHeightPercent);
    }

    return bottomContainer;
}

std::shared_ptr<Containers> HUD::createTpsContainer(
    int screenWidth,
    int screenHeight,
    float bottomHeight,
    float bottomHeightPercent)
{
    float containerWidth = 200.0f;
    float containerHeight = 120.0f;
    (void)bottomHeight;
    (void)bottomHeightPercent;

    auto tpsContainer = addContainer(
        "tps_container",
        screenWidth - containerWidth, 0,
        containerWidth, containerHeight,
        {40, 40, 40, 200}
    );

    if (tpsContainer) {
        float widthPercent = (containerWidth / screenWidth) * 100.0f;
        float heightPercent = (containerHeight / screenHeight) * 100.0f;

        tpsContainer->setRelativePosition(
            100.0f - widthPercent,
            0,
            widthPercent,
            heightPercent);
    }

    return tpsContainer;
}

std::shared_ptr<Containers> HUD::createSecurityContainer(
    int screenWidth,
    int screenHeight,
    float bottomHeight,
    float bottomHeightPercent)
{
    float containerWidth = 200.0f;
    float containerHeight = 80.0f;
    (void)bottomHeight;
    (void)bottomHeightPercent;

    auto securityContainer = addContainer(
        "security_container",
        screenWidth - containerWidth, 125,
        containerWidth, containerHeight,
        {40, 40, 40, 200}
    );

    if (securityContainer) {
        float widthPercent = (containerWidth / screenWidth) * 100.0f;
        float heightPercent = (containerHeight / screenHeight) * 100.0f;

        securityContainer->setRelativePosition(
            100.0f - widthPercent,
            6.0f,
            widthPercent,
            heightPercent);

        securityContainer->addButtonPercent(
            "security_button",
            5.0f, 10.0f,
            90.0f, 80.0f,
            "SECURITY SYNCHRONIZATION",
            [this]() {
                this->_gameInfos->securityActualisation();
            },
            {80, 80, 240, 255},
            {120, 120, 255, 255},
            {60, 60, 200, 255},
            {255, 255, 255, 255}
        );
    }

    return securityContainer;
}

std::shared_ptr<Containers> HUD::createServerMessagesContainer(
    int screenWidth,
    int screenHeight,
    float bottomHeight,
    float bottomHeightPercent)
{
    (void)bottomHeight;
    (void)bottomHeightPercent;
    float containerWidth = screenWidth * 0.5f;
    float containerHeight = screenHeight * 0.4f;
    float containerX = (screenWidth - containerWidth) / 2.0f;
    float containerY = (screenHeight - containerHeight) / 2.0f;

    auto serverMessagesContainer = addContainer(
        "server_messages_container",
        containerX, containerY,
        containerWidth, containerHeight,
        {40, 40, 40, 220}
    );

    if (serverMessagesContainer) {
        float xPercent = (containerX / screenWidth) * 100.0f;
        float yPercent = (containerY / screenHeight) * 100.0f;
        float widthPercent = (containerWidth / screenWidth) * 100.0f;
        float heightPercent = (containerHeight / screenHeight) * 100.0f;

        serverMessagesContainer->setRelativePosition(
            xPercent,
            yPercent,
            widthPercent,
            heightPercent);

        serverMessagesContainer->addButtonPercent(
            "close_server_messages_button",
            92.0f, 2.0f,
            6.0f, 8.0f,
            "X",
            [this]() {
                auto container = this->getServerMessagesContainer();
                if (!container)
                    return;

                container->setVisible(false);
            },
            {200, 50, 50, 255},
            {255, 80, 80, 255},
            {150, 30, 30, 255},
            {255, 255, 255, 255}
        );
    }

    return serverMessagesContainer;
}

std::shared_ptr<Containers> HUD::createMapInfoContainer()
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return nullptr;

    FloatRect bottomBounds = bottomContainer->getBounds();
    float buttonX = bottomBounds.x + (bottomBounds.width * 0.945f);
    float buttonY = bottomBounds.y + (bottomBounds.height * 0.025f);

    float containerWidth = 350.0f;
    float containerHeight = 250.0f;
    float containerX = buttonX - containerWidth + (bottomBounds.width * 0.05f) + 10.0f;
    float containerY = buttonY - containerHeight - 5.0f;

    auto mapInfoContainer = addContainer(
        "map_info_container",
        containerX, containerY,
        containerWidth, containerHeight,
        {40, 40, 40, 240}
    );

    if (mapInfoContainer) {
        mapInfoContainer->addTextPercent(
            "map_info_title", 5.0f, 2.5f, "Map Information", 12.0f,
            {255, 255, 255, 255}
        );

        mapInfoContainer->addTextPercent(
            "map_info_content", 5.0f, 15.5f, "Loading map data...", 6.5f,
            {200, 200, 200, 255}
        );

        mapInfoContainer->setVisible(false);
    }

    return mapInfoContainer;
}

std::pair<float, float> HUD::calculateContentMetrics(
    std::shared_ptr<Containers> container,
    const std::unordered_map<std::string, float> &initialYPositions)
{
    FloatRect containerBounds = container->getBounds();
    float maxY = containerBounds.y;

    for (const auto& pair : initialYPositions) {
        auto element = container->getElement(pair.first);
        if (element) {
            FloatRect bounds = element->getBounds();
            float elemBottom = bounds.y + bounds.height;
            if (elemBottom > maxY) maxY = elemBottom;
        }
    }

    float contentHeight = maxY - containerBounds.y;
    float teamCount = 0;

    for (int i = 0; i < 100; i++) {
        if (container->getElement("team_display_" + std::to_string(i) + "_title"))
            teamCount++;
    }

    float extraSpace = teamCount > 10 ? (teamCount - 10) * 20.0f : 100.0f;
    contentHeight += extraSpace;

    return {contentHeight, teamCount};
}

void HUD::updateElementPositions(
    std::shared_ptr<Containers> container,
    const std::unordered_map<std::string, float>& initialYPositions,
    float offset)
{
    for (int i = 0; i < 100; i++) {
        std::string idBase = "team_display_" + std::to_string(i);

        auto separatorElem = container->getElement(idBase + "_separator");
        if (separatorElem && initialYPositions.find(idBase + "_separator") !=
            initialYPositions.end()) {
            float initialY = initialYPositions.at(idBase + "_separator");
            separatorElem->setPosition(separatorElem->getBounds().x, initialY + offset);
        }

        auto titleElem = container->getElement(idBase + "_title");
        if (titleElem && initialYPositions.find(idBase + "_title") !=
            initialYPositions.end()) {
            float initialY = initialYPositions.at(idBase + "_title");
            titleElem->setPosition(titleElem->getBounds().x, initialY + offset);

            auto statsElem = container->getElement(idBase + "_stats");
            if (statsElem && initialYPositions.find(idBase + "_stats") !=
                initialYPositions.end()) {
                float initialStatsY = initialYPositions.at(idBase + "_stats");
                statsElem->setPosition(statsElem->getBounds().x, initialStatsY + offset);
            }
        }

        for (int j = 0; j < 50; j++) {
            std::string playerID = idBase + "_player_" + std::to_string(j);
            auto playerElem = container->getElement(playerID);
            if (playerElem && initialYPositions.find(playerID) !=
                initialYPositions.end()) {
                float initialY = initialYPositions.at(playerID);
                playerElem->setPosition(playerElem->getBounds().x, initialY + offset);
            }
        }
    }
}

zappy::structs::Player HUD::getPlayerById(int playerId) const
{
    const auto& players = _gameInfos->getPlayers();

    for (const auto& player : players) {
        if (player.number == playerId)
            return player;
    }

    return zappy::structs::Player();
}

void HUD::initPlayerInventoryDisplay(int playerId)
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    clearPlayerInventoryElements();

    const zappy::structs::Player player = getPlayerById(playerId);
    if (player.number <= 0)
        return;

    bottomContainer->addTextPercent(
        "player_info_title",
        52.5f, 2.5f,
        "PLAYER INFORMATION",
        8.0f,
        {255, 255, 255, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_separator",
        52.5f, 10.0f,
        std::string(70, '-'),
        2.0f,
        {150, 150, 150, 200}
    );

    bottomContainer->addTextPercent(
        "player_info_id",
        52.5f, 27.5f,
        "ID: " + std::to_string(player.number),
        7.0f,
        {220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_team",
        52.5f, 40.5f,
        "Team: " + player.teamName,
        7.0f,
        {220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_level",
        52.5f, 53.5f,
        "Level: " + std::to_string(player.level),
        7.0f,
        {220, 220, 220, 255}
    );

    addIncrementDecrementButtons(bottomContainer, playerId);

    std::string orientationStr;
    switch (player.orientation) {
        case 1: orientationStr = "North"; break;
        case 2: orientationStr = "East"; break;
        case 3: orientationStr = "South"; break;
        case 4: orientationStr = "West"; break;
        default: orientationStr = "Unknown";
    }

    bottomContainer->addTextPercent(
        "player_info_position",
        62.5f, 27.5f,
        "Position: [" + std::to_string(player.x) + ", " + std::to_string(player.y) + "]",
        7.0f,
        {220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_orientation",
        62.5f, 40.5f,
        "Orientation: " + orientationStr,
        7.0f,
        {220, 220, 220, 255}
    );

    bool inRitual = isPlayerInIncantation(player.number);
    bottomContainer->addTextPercent(
        "player_info_ritual",
        62.5f, 53.5f,
        "Ritual: " + std::string(inRitual ? "Yes" : "No"),
        7.0f,
        inRitual ? Color32{255, 215, 0, 255} : Color32{220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "inventory_title",
        75.0f, 2.5f,
        "INVENTORY",
        8.0f,
        {255, 255, 255, 255}
    );

    bottomContainer->addTextPercent(
        "inventory_separator",
        75.0f, 10.0f,
        std::string(70, '-'),
        2.0f,
        {150, 150, 150, 200}
    );

    bottomContainer->addTextPercent(
        "inventory_food",
        82.0f, 15.0f,
        "Food: " + std::to_string(player.inventory.food),
        7.5f,
        {255, 215, 0, 255}
    );

    float yPosCol1 = 27.5f;
    float xPosCol1 = 75.0f;

    bottomContainer->addTextPercent(
        "inventory_linemate",
        xPosCol1, yPosCol1,
        "Linemate: " + std::to_string(player.inventory.linemate),
        7.0f,
        {200, 200, 200, 255}
    );
    yPosCol1 += 13.0f;

    bottomContainer->addTextPercent(
        "inventory_deraumere",
        xPosCol1, yPosCol1,
        "Deraumere: " + std::to_string(player.inventory.deraumere),
        7.0f,
        {65, 105, 225, 255}
    );
    yPosCol1 += 13.0f;

    bottomContainer->addTextPercent(
        "inventory_sibur",
        xPosCol1, yPosCol1,
        "Sibur: " + std::to_string(player.inventory.sibur),
        7.0f,
        {50, 205, 50, 255}
    );

    float yPosCol2 = 27.5f;
    float xPosCol2 = 88.0f;

    bottomContainer->addTextPercent(
        "inventory_mendiane",
        xPosCol2, yPosCol2,
        "Mendiane: " + std::to_string(player.inventory.mendiane),
        7.0f,
        {255, 165, 0, 255}
    );
    yPosCol2 += 13.0f;

    bottomContainer->addTextPercent(
        "inventory_phiras",
        xPosCol2, yPosCol2,
        "Phiras: " + std::to_string(player.inventory.phiras),
        7.0f,
        {138, 43, 226, 255}
    );
    yPosCol2 += 13.0f;

    bottomContainer->addTextPercent(
        "inventory_thystame",
        xPosCol2, yPosCol2,
        "Thystame: " + std::to_string(player.inventory.thystame),
        7.0f,
        {255, 20, 147, 255}
    );

    bottomContainer->addButtonPercent(
        "kill_player_button",
        87.5f, 2.5f,
        5.0f, 10.0f,
        "KILL",
        [this]() {
            _gameInfos->killPlayer(0);
        },
        {200, 50, 50, 255},
        {255, 80, 80, 255},
        {150, 30, 30, 255},
        {255, 255, 255, 255}
    );
}

std::string HUD::_camModeToText(zappy::gui::CameraMode cameraMode, bool isGamePadAvailable)
{
    std::string changeCamKey = "";
    changeCamKey = isGamePadAvailable ? "LB" : "TAB";
    switch (cameraMode)
    {
    case zappy::gui::CameraMode::FREE:
        return "Free\nChange camera mod: " + changeCamKey;
    case zappy::gui::CameraMode::PLAYER:
        return "Player\nChange camera mod: " + changeCamKey;
    case zappy::gui::CameraMode::TARGETED:
        return "Targeted\nChange camera mod: " + changeCamKey;
    default:
        return "Unknown\nChange camera mod: " + changeCamKey;
    }
    return "Unknown\nChange camera mod: " + changeCamKey;
}

std::string HUD::_camKeyHelp(zappy::gui::CameraMode cameraMode, bool isGamePadAvailable)
{
    if (isGamePadAvailable) {
        switch (cameraMode)
        {
        case zappy::gui::CameraMode::FREE:
            return "Right joystick =\n"
                     "Change camera direction\n"
                     "Left joystick = Move camera x z\n"
                     "RT | LT = Move camera y\n"
                     "Select = Toggle HUD\n";
        case zappy::gui::CameraMode::PLAYER:
            return "UP | DOWN =\n"
                     "Next / Previous player\n"
                     "Right joystick =\n"
                     "Change camera direction\n"
                     "Select = Toggle HUD\n";
        case zappy::gui::CameraMode::TARGETED:
            return "Right joystick =\n"
                     "Rotate camera around center\n"
                     "RT | LT = Zoom / Unzoom\n"
                     "Select = Toggle HUD\n";
        default:
            return "Unknown";
        }
    }
    switch (cameraMode)
    {
    case zappy::gui::CameraMode::FREE:
        return "Z | Q | S | D = Move camera\n"
                 "UP | DOWN | RIGHT | LEFT =\n"
                 "Change camera direction\n"
                 "H = Toggle HUD\n";
    case zappy::gui::CameraMode::PLAYER:
        return "UP | DOWN =\n"
                 "Next / Previous player\n"
                 "RIGHT | LEFT =\n"
                 "Change camera direction\n"
                 "H = Toggle HUD\n";
    case zappy::gui::CameraMode::TARGETED:
        return "UP | DOWN | RIGHT | LEFT =\n"
                 "Rotate camera around center\n"
                 "SCROLL = Zoom / Unzoom\n"
                 "H = Toggle HUD\n";
    default:
        return "Unknown";
    }
    return "Unknown";
}

std::string HUD::_mapGlobalInfo(std::shared_ptr<GameInfos> gameInfos)
{
    auto mapSize = gameInfos->getMapSize();
    std::string info = "";

    info += "Map Size: " + std::to_string(mapSize.first) + " x " +
        std::to_string(mapSize.second) + "\n";

    if (this->_performanceMode || (mapSize.first >= 50 && mapSize.second >= 50)) {
        info += "Food: X   Egg: X\n";
        info += "Linemate: X\nDeraumere: X\n";
        info += "Sibur: X\nMendiane: X\n";
        info += "Phiras: X\nThystame: X\n";
        info += "Total ressources: X\n";
    } else {
        int totalEgg = gameInfos->getTotalEggs();
        int totalFood = gameInfos->getTotalFood();
        int totalLinemate = gameInfos->getTotalLinemate();
        int totalDeraumere = gameInfos->getTotalDeraumere();
        int totalSibur = gameInfos->getTotalSibur();
        int totalMendiane = gameInfos->getTotalMendiane();
        int totalPhiras = gameInfos->getTotalPhiras();
        int totalThystame = gameInfos->getTotalThystame();

        info += "Food: " + std::to_string(totalFood) + "   Egg: " +
            std::to_string(totalEgg) + "\n";
        info += "Linemate: " + std::to_string(totalLinemate) + "\nDeraumere: " +
            std::to_string(totalDeraumere) + "\n";
        info += "Sibur: " + std::to_string(totalSibur) + "\nMendiane: " +
            std::to_string(totalMendiane) + "\n";
        info += "Phiras: " + std::to_string(totalPhiras) + "\nThystame: " +
            std::to_string(totalThystame) + "\n";
        info += "Total ressources: " + std::to_string(totalFood + totalLinemate +
            totalDeraumere + totalSibur + totalMendiane + totalPhiras + totalThystame) + "\n";
    }

    const auto& players = gameInfos->getPlayers();
    const auto& teams = gameInfos->getTeamNames();

    info += "Total teams: " + std::to_string(teams.size()) + "\n";
    info += "Total players: " + std::to_string(players.size()) + "\n";

    int max_level = 1;
    for (const auto& player : players) {
        if (player.level > max_level)
            max_level = player.level;
    }
    info += "Player Level Max: " + std::to_string(max_level) + "\n";

    return info;
}

void HUD::updateHelpInformationHUD(zappy::gui::CameraMode cameraMode)
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    InputType lastInputType = this->_display->getLastInputType();
    bool isUsingGamepad = (lastInputType == InputType::GAMEPAD);

    auto camInfoElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("cam_info_mode"));
    auto strCamMode = this->_camModeToText(cameraMode, isUsingGamepad);
    if (camInfoElem && camInfoElem->getText() != "Camera mod: " + strCamMode) {
        camInfoElem->setText("Camera mod: " + strCamMode);
        return;
    }
    auto keyHelp = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("help_cam_key"));
    auto camHelpKey = this->_camKeyHelp(cameraMode, isUsingGamepad);
    if (keyHelp && keyHelp->getText() != camHelpKey) {
        keyHelp->setText(camHelpKey);
        return;
    }
}

void HUD::initMapInfoDisplay()
{
    auto mapInfoContainer = getMapInfoContainer();
    if (!mapInfoContainer)
        return;

    mapInfoContainer->setVisible(false);
}

void HUD::initMapInfoButton()
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    bottomContainer->addButtonPercent(
        "map_info_button", 94.5f, 2.5f, 5.0f, 15.0f,
        "Map Info",
        []() {
        },
        {70, 130, 180, 220}, {100, 160, 210, 255}, {100, 160, 210, 255}, {255, 255, 255, 255}
    );
}

void HUD::updateMapInfoDisplay()
{
    auto bottomContainer = getBottomContainer();
    auto mapInfoContainer = getMapInfoContainer();
    if (!bottomContainer || !mapInfoContainer || !_gameInfos)
        return;

    auto mapInfoButton = std::dynamic_pointer_cast<Button>(
        bottomContainer->getElement("map_info_button"));

    bool isButtonHovered = false;
    if (mapInfoButton) {
        Vector2f mousePos = _display->getMousePosition();
        isButtonHovered = mapInfoButton->contains(mousePos.x, mousePos.y);
    }

    auto mapInfoTextElem = std::dynamic_pointer_cast<Text>(
        mapInfoContainer->getElement("map_info_content"));

    if (!mapInfoTextElem) {
        return;
    }

    if (isButtonHovered) {
        auto mapInfoText = this->_mapGlobalInfo(_gameInfos);
        if (mapInfoTextElem->getText() != mapInfoText) {
            mapInfoTextElem->setText(mapInfoText);
        }
        mapInfoContainer->setVisible(true);
    } else {
        mapInfoContainer->setVisible(false);
    }
}

void HUD::updatePlayerInventoryDisplay(int playerId, zappy::gui::CameraMode cameraMode)
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    if (cameraMode != zappy::gui::CameraMode::PLAYER) {
        clearPlayerInventoryElements();
        return;
    }

    const zappy::structs::Player player = getPlayerById(playerId);
    if (player.number <= 0) {
        clearPlayerInventoryElements();
        return;
    }

    auto killButton = std::dynamic_pointer_cast<Button>(
        bottomContainer->getElement("kill_player_button"));
    if (killButton) {
        killButton->setCallback([this, playerId]() {
            _gameInfos->killPlayer(playerId);
        });
    }

    auto titleElem = bottomContainer->getElement("player_info_title");
    if (!titleElem) {
        initPlayerInventoryDisplay(playerId);
        return;
    }

    auto levelElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_level"));
    if (levelElem) {
        levelElem->setText("Level: " + std::to_string(player.level));
    }

    auto teamElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_team"));
    if (teamElem) {
        teamElem->setText("Team: " + player.teamName);
    }

    auto idElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_id"));
    if (idElem) {
        idElem->setText("ID: " + std::to_string(player.number));
    }

    bool inRitual = isPlayerInIncantation(player.number);
    auto ritualElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_ritual"));
    if (ritualElem) {
        ritualElem->setText("Ritual: " + std::string(inRitual ? "Yes" : "No"));
        ritualElem->setColor(inRitual ? Color32{255, 215, 0, 255} :
                                        Color32{220, 220, 220, 255});
    }

    auto posElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_position"));
    if (posElem) {
        posElem->setText("Position: [" + std::to_string(player.x) + ", " +
                         std::to_string(player.y) + "]");
    }

    std::string orientationStr;
    switch (player.orientation) {
        case 1: orientationStr = "North"; break;
        case 2: orientationStr = "East"; break;
        case 3: orientationStr = "South"; break;
        case 4: orientationStr = "West"; break;
        default: orientationStr = "Unknown";
    }

    auto orientationElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_orientation"));
    if (orientationElem) {
        orientationElem->setText("Orientation: " + orientationStr);
    }

    auto foodElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_food"));
    if (foodElem) {
        foodElem->setText("Food: " + std::to_string(player.inventory.food));
    }

    auto linemateElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_linemate"));
    if (linemateElem) {
        linemateElem->setText("Linemate: " + std::to_string(player.inventory.linemate));
    }

    auto deraumereElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_deraumere"));
    if (deraumereElem) {
        deraumereElem->setText("Deraumere: " + std::to_string(player.inventory.deraumere));
    }

    auto siburElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_sibur"));
    if (siburElem) {
        siburElem->setText("Sibur: " + std::to_string(player.inventory.sibur));
    }

    auto mendianeElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_mendiane"));
    if (mendianeElem) {
        mendianeElem->setText("Mendiane: " + std::to_string(player.inventory.mendiane));
    }

    auto phirasElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_phiras"));
    if (phirasElem) {
        phirasElem->setText("Phiras: " + std::to_string(player.inventory.phiras));
    }

    auto thystameElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_thystame"));
    if (thystameElem) {
        thystameElem->setText("Thystame: " + std::to_string(player.inventory.thystame));
    }
}

void HUD::setResetCameraCallback(std::function<void()> resetFunc)
{
    _resetCameraFunc = resetFunc;
}

void HUD::initTpsSlider(
    std::shared_ptr<GameInfos> gameInfos,
    std::shared_ptr<IDisplay> display,
    std::shared_ptr<IAudio> audio)
{
    auto tpsContainer = getTpsContainer();
    if (!tpsContainer || !gameInfos || !display || !audio)
        return;

    tpsContainer->addSliderPercent(
        "tps_slider", 5.0f, 15.0f, 150.0f, 100.0f,
        1.0f, 1000.0f, gameInfos->getTimeUnit(),
        "Frequency (TPS): ",
        [gameInfos, display, audio](float value) {
            gameInfos->setTimeUnit(static_cast<int>(value), true);
        }
    );
}

void HUD::updateTpsSlider(std::shared_ptr<GameInfos> gameInfos)
{
    auto tpsContainer = getTpsContainer();
    if (!tpsContainer || !gameInfos)
        return;

    auto slider = std::dynamic_pointer_cast<Slider>(
        tpsContainer->getElement("tps_slider"));

    if (!slider)
        return;

    static std::chrono::steady_clock::time_point lastUpdateTime;
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - lastUpdateTime;
    if (elapsed.count() < 5.0f)
        return;

    lastUpdateTime = now;
    if (slider->isDragging())
        return;

    slider->setValue(static_cast<float>(gameInfos->getTimeUnit()));
}

void HUD::initServerMessagesDisplay(std::shared_ptr<GameInfos> gameInfos)
{
    auto serverMessagesContainer = getServerMessagesContainer();
    if (!serverMessagesContainer || !gameInfos)
        return;

    serverMessagesContainer->addTextPercent(
        "server_messages_popup_title",
        5.0f, 5.0f,
        "Server Messages",
        12.0f,
        {245, 224, 80, 255}
    );

    serverMessagesContainer->addTextPercent(
        "server_messages_separator",
        5.0f, 15.0f,
        std::string(80, '-'),
        3.0f,
        {150, 150, 150, 200}
    );

    std::string text = "";
    const auto& messages = gameInfos->getServerMessages();

    size_t startIndex = messages.size() > 5 ? messages.size() - 5 : 0;
    for (size_t i = startIndex; i < messages.size(); ++i) {
        text += messages[i] + "\n";
    }

    if (text.empty()) {
        text = "No server messages yet...";
        serverMessagesContainer->setVisible(false);
    }

    serverMessagesContainer->addTextPercent(
        "server_messages_content",
        5.0f, 20.0f,
        text,
        8.0f,
        {245, 224, 80, 255}
    );
}

void HUD::updateServerMessagesDisplay(std::shared_ptr<GameInfos> gameInfos)
{
    auto serverMessagesContainer = getServerMessagesContainer();
    if (!serverMessagesContainer || !gameInfos)
        return;

    auto contentElem = std::dynamic_pointer_cast<Text>(
        serverMessagesContainer->getElement("server_messages_content"));

    if (!contentElem)
        return;

    const auto& messages = gameInfos->getServerMessages();
    std::string text = "";

    size_t startIndex = messages.size() > 5 ? messages.size() - 5 : 0;
    for (size_t i = startIndex; i < messages.size(); ++i) {
        text += messages[i] + "\n";
    }

    if (text.empty()) {
        text = "No server messages yet...";
        serverMessagesContainer->setVisible(false);
        return;
    }

    contentElem->setText(text);
}

bool HUD::isPlayerInIncantation(int playerId) const
{
    const auto& incantations = _gameInfos->getIncantations();

    for (const auto& incantation : incantations) {
        for (int id : incantation.players) {
            if (id == playerId) {
                return true;
            }
        }
    }

    return false;
}

void HUD::displayWinMessage(const std::string& teamName)
{
    _showVictoryMessage = true;
    _winningTeam = teamName;
    _victoryColor = _gameInfos->getTeamColor(teamName);
}

void HUD::onGameEvent(GameEventType eventType, const std::string& teamName)
{
    switch (eventType) {
        case GameEventType::TEAM_WIN:
            _showVictoryMessage = true;
            _winningTeam = teamName;
            _victoryColor = _gameInfos->getTeamColor(teamName);
            break;
        default:
            break;
    }
}

void HUD::setSelectedTile(int x, int y)
{
    if (_selectedTile.first != x || _selectedTile.second != y) {
        _selectedTile = {x, y};

        if (x >= 0 && y >= 0) {
            updateTileResourceDisplay(x, y);
        } else {
            clearTileResourceElements();
        }
    }
}

void HUD::initTileResourceDisplay()
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    clearTileResourceElements();

    bottomContainer->addTextPercent(
        "tile_resources_title",
        30.0f, 2.5f,
        "TILE RESOURCES",
        8.0f,
        {255, 255, 255, 255}
    );

    bottomContainer->addTextPercent(
        "tile_resources_separator",
        30.0f, 10.0f,
        std::string(130, '-'),
        2.0f,
        {150, 150, 150, 200}
    );

    float yPosCol1 = 27.5f;
    float xPosCol1 = 30.0f;

    bottomContainer->addTextPercent(
        "tile_resource_food",
        xPosCol1 + 7.5f, 15.0f,
        "Food: 0",
        7.5f,
        {255, 215, 0, 255}
    );

    bottomContainer->addTextPercent(
        "tile_resource_linemate",
        xPosCol1, yPosCol1,
        "Linemate: 0",
        7.0f,
        {200, 200, 200, 255}
    );
    yPosCol1 += 13.0f;

    bottomContainer->addTextPercent(
        "tile_resource_deraumere",
        xPosCol1, yPosCol1,
        "Deraumere: 0",
        7.0f,
        {65, 105, 225, 255}
    );
    yPosCol1 += 13.0f;

    bottomContainer->addTextPercent(
        "tile_resource_sibur",
        xPosCol1, yPosCol1,
        "Sibur: 0",
        7.0f,
        {50, 205, 50, 255}
    );

    float yPosCol2 = 27.5f;
    float xPosCol2 = 45.0f;

    bottomContainer->addTextPercent(
        "tile_resource_mendiane",
        xPosCol2, yPosCol2,
        "Mendiane: 0",
        7.0f,
        {255, 165, 0, 255}
    );
    yPosCol2 += 13.0f;

    bottomContainer->addTextPercent(
        "tile_resource_phiras",
        xPosCol2, yPosCol2,
        "Phiras: 0",
        7.0f,
        {138, 43, 226, 255}
    );
    yPosCol2 += 13.0f;

    bottomContainer->addTextPercent(
        "tile_resource_thystame",
        xPosCol2, yPosCol2,
        "Thystame: 0",
        7.0f,
        {255, 20, 147, 255}
    );

    bottomContainer->addButtonPercent(
        "tile_food_increment_btn",
        34.9f, 15.0f,
        1.0f, 6.5f,
        "+",
        [this]() {
            this->_gameInfos->incrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 0);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_food_decrement_btn",
        36.0f, 15.0f,
        1.0f, 6.5f,
        "-",
        [this]() {
            this->_gameInfos->decrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 0);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_linemate_increment_btn",
        27.4f, 27.5f,
        1.0f, 6.5f,
        "+",
        [this]() {
            this->_gameInfos->incrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 1);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_linemate_decrement_btn",
        28.5f, 27.5f,
        1.0f, 6.5f,
        "-",
        [this]() {
            this->_gameInfos->decrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 1);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_deraumere_increment_btn",
        27.4f, 40.5f,
        1.0f, 6.5f,
        "+",
        [this]() {
            this->_gameInfos->incrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 2);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_deraumere_decrement_btn",
        28.5f, 40.5f,
        1.0f, 6.5f,
        "-",
        [this]() {
            this->_gameInfos->decrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 2);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_sibur_increment_btn",
        27.4f, 53.5f,
        1.0f, 6.5f,
        "+",
        [this]() {
            this->_gameInfos->incrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 3);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_sibur_decrement_btn",
        28.5f, 53.5f,
        1.0f, 6.5f,
        "-",
        [this]() {
            this->_gameInfos->decrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 3);
        }
    );

        bottomContainer->addButtonPercent(
        "tile_mendiane_increment_btn",
        42.4f, 27.5f,
        1.0f, 6.5f,
        "+",
        [this]() {
            this->_gameInfos->incrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 4);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_mendiane_decrement_btn",
        43.5f, 27.5f,
        1.0f, 6.5f,
        "-",
        [this]() {
            this->_gameInfos->decrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 4);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_phiras_increment_btn",
        42.4f, 40.5f,
        1.0f, 6.5f,
        "+",
        [this]() {
            this->_gameInfos->incrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 5);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_phiras_decrement_btn",
        43.5f, 40.5f,
        1.0f, 6.5f,
        "-",
        [this]() {
            this->_gameInfos->decrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 5);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_thystame_increment_btn",
        42.4f, 53.5f,
        1.0f, 6.5f,
        "+",
        [this]() {
            this->_gameInfos->incrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 6);
        }
    );

    bottomContainer->addButtonPercent(
        "tile_thystame_decrement_btn",
        43.5f, 53.5f,
        1.0f, 6.5f,
        "-",
        [this]() {
            this->_gameInfos->decrementTileInventoryItem(this->_selectedTile.first,
                this->_selectedTile.second, 6);
        }
    );
}

void HUD::updateTileResourceDisplay(int x, int y)
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer || !_gameInfos)
        return;

    auto titleElem = bottomContainer->getElement("tile_resources_title");
    if (!titleElem)
        initTileResourceDisplay();

    zappy::structs::Tile tile = _gameInfos->getTile(this->_selectedTile.first,
        this->_selectedTile.second);

    auto tileResourcesTitle = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("tile_resources_title"));
    if (tileResourcesTitle) {
        tileResourcesTitle->setText("TILE RESOURCES (" + std::to_string(x) + ", "
        + std::to_string(y) + ")");
    }

    auto foodElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("tile_resource_food"));
    if (foodElem) {
        foodElem->setText("Food: " + std::to_string(tile.food));
    }

    auto linemateElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("tile_resource_linemate"));
    if (linemateElem) {
        linemateElem->setText("Linemate: " + std::to_string(tile.linemate));
    }

    auto deraumereElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("tile_resource_deraumere"));
    if (deraumereElem) {
        deraumereElem->setText("Deraumere: " + std::to_string(tile.deraumere));
    }

    auto siburElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("tile_resource_sibur"));
    if (siburElem) {
        siburElem->setText("Sibur: " + std::to_string(tile.sibur));
    }

    auto mendianeElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("tile_resource_mendiane"));
    if (mendianeElem) {
        mendianeElem->setText("Mendiane: " + std::to_string(tile.mendiane));
    }

    auto phirasElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("tile_resource_phiras"));
    if (phirasElem) {
        phirasElem->setText("Phiras: " + std::to_string(tile.phiras));
    }

    auto thystameElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("tile_resource_thystame"));
    if (thystameElem) {
        thystameElem->setText("Thystame: " + std::to_string(tile.thystame));
    }
}

void HUD::clearTileResourceElements()
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    std::vector<std::string> elementIds = {
        "tile_resources_title",
        "tile_resources_separator",
        "tile_resource_food", "tile_resource_linemate", "tile_resource_deraumere",
        "tile_resource_sibur", "tile_resource_mendiane", "tile_resource_phiras",
        "tile_resource_thystame",
        "tile_food_increment_btn", "tile_food_decrement_btn",
        "tile_linemate_increment_btn", "tile_linemate_decrement_btn",
        "tile_deraumere_increment_btn", "tile_deraumere_decrement_btn",
        "tile_sibur_increment_btn", "tile_sibur_decrement_btn",
        "tile_mendiane_increment_btn", "tile_mendiane_decrement_btn",
        "tile_phiras_increment_btn", "tile_phiras_decrement_btn",
        "tile_thystame_increment_btn", "tile_thystame_decrement_btn",
    };

    for (const auto& id : elementIds) {
        bottomContainer->removeElement(id);
    }
}

bool HUD::isMouseOverHUD() const
{
    if (!_display)
        return false;

    Vector2f mousePos = _display->getMousePosition();

    for (const auto& pair : _containers) {
        if (pair.second && pair.second->isVisible()) {
            if (pair.second->contains(mousePos.x, mousePos.y)) {
                return true;
            }
        }
    }

    if (_help->containsPoint(mousePos.x, mousePos.y) ||
        _settings->containsPoint(mousePos.x, mousePos.y))
        return true;

    return false;
}

void HUD::initFpsDisplay()
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    bottomContainer->addTextPercent(
        "fps_display",
        95.25f, 32.5f,
        "FPS: 60", 6.0f,
        {255, 255, 255, 255}
    );

    bottomContainer->addTextPercent(
        "cycle_display",
        95.25f, 37.5f,
        "Cycle: 00h", 6.0f,
        {255, 255, 255, 255}
    );
}

void HUD::updateFpsDisplay()
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer || !_display)
        return;

    int fps = _display->getFPS();
    auto fpsElement = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("fps_display"));
    if (fpsElement) {
        fpsElement->setText("FPS: " + std::to_string(fps));
    }

    auto cycleElement = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("cycle_display"));
    if (cycleElement) {
        int hours = static_cast<int>(fmodf(_display->getTime() /
            zappy::gui::DURATION_DAYNIGHT_CYCLE * 24.0f + 12.0f, 24.0f));

        cycleElement->setText("Cycle: " + std::to_string(hours) + "h");
    }
}

void HUD::updateTeamHoverDetection()
{
    static std::chrono::steady_clock::time_point lastCheckTime;
    static std::string lastHoveredTeam;
    static bool shouldDisplay = false;

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - lastCheckTime;

    if (elapsed.count() < 0.1f) {
        if (shouldDisplay && !lastHoveredTeam.empty())
            showTeamDetailsContainer(lastHoveredTeam);
        else
            hideTeamDetailsContainer();
        return;
    }
    lastCheckTime = now;

    if (!_gameInfos || !_display)
        return;

    Vector2f mousePos = _display->getMousePosition();
    auto sideContainer = getSideContainer();
    if (!sideContainer)
        return;

    std::string currentHoveredTeam = "";
    const std::vector<std::string> teams = _gameInfos->getTeamNames();

    for (int i = static_cast<int>(teams.size()) - 1; i >= 0; i--) {
        std::string teamId = "team_display_" + std::to_string(i);
        const std::string& teamName = teams[i];

        auto titleElement = sideContainer->getElement(teamId + "_title");
        if (titleElement && titleElement->contains(mousePos.x, mousePos.y)) {
            currentHoveredTeam = teamName;
            break;
        }

        auto checkboxElement = sideContainer->getElement(teamId + "_checkbox");
        if (checkboxElement && checkboxElement->contains(mousePos.x, mousePos.y)) {
            currentHoveredTeam = teamName;
            break;
        }
    }

    if (currentHoveredTeam != _hoveredTeam) {
        _hoveredTeam = currentHoveredTeam;
        lastHoveredTeam = _hoveredTeam;
        shouldDisplay = !_hoveredTeam.empty();
        if (shouldDisplay)
            showTeamDetailsContainer(_hoveredTeam);
        else
            hideTeamDetailsContainer();
    } else if (!_hoveredTeam.empty()) {
        lastHoveredTeam = _hoveredTeam;
        shouldDisplay = true;
        showTeamDetailsContainer(_hoveredTeam);
    } else {
        shouldDisplay = false;
        hideTeamDetailsContainer();
    }
}

void HUD::createTeamDetailsContainer()
{
    if (_teamDetailsContainer)
        return;

    Vector2i screenDimensions = _display->getScreenSize();
    float screenWidth = static_cast<float>(screenDimensions.x);
    float screenHeight = static_cast<float>(screenDimensions.y);

    float sideWidthPercent = 15.0f;
    float bottomHeightPercent = 20.0f;
    float sideWidth = (screenWidth * sideWidthPercent) / 100.0f;
    float squareSize = sideWidth;

    float containerWidth = screenWidth * 0.15f;
    float containerHeight = screenHeight - squareSize -
        (screenHeight * bottomHeightPercent / 100.0f);
    float containerX = sideWidth;
    float containerY = squareSize;

    _teamDetailsContainer = std::make_shared<Containers>(
        _display,
        _audio,
        containerX,
        containerY,
        containerWidth,
        containerHeight,
        Color32{40, 40, 40, 220}
    );

    _teamDetailsContainer->setRelativePosition(
        sideWidthPercent,
        sideWidthPercent,
        sideWidthPercent,
        100.0f - sideWidthPercent - bottomHeightPercent
    );

    _teamDetailsContainer->setVisible(false);
}

void HUD::showTeamDetailsContainer(const std::string& teamName)
{
    if (!_teamDetailsContainer)
        createTeamDetailsContainer();

    _teamDetailsContainer->clearElements();

    _teamDetailsContainer->addTextPercent(
        "details_title",
        5.0f,
        5.0f,
        "Team Details:",
        4.0f,
        {255, 255, 255, 255}
    );

    Color32 teamColor = _gameInfos->getTeamColor(teamName);
    _teamDetailsContainer->addTextPercent(
        "team_name",
        5.0f,
        10.0f,
        teamName,
        4.0f,
        teamColor
    );

    const auto& players = _gameInfos->getPlayers();
    std::vector<zappy::structs::Player> teamPlayers;
    std::vector<int> levelCount(9, 0);
    int totalPlayers = 0;
    int totalLevels = 0;

    zappy::structs::Inventory teamResources;
    int alivePlayersCount = 0;

    for (const auto& player : players) {
        if (player.teamName == teamName) {
            teamPlayers.push_back(player);
            totalPlayers++;
            alivePlayersCount++;
            totalLevels += player.level;
            if (player.level >= 1 && player.level <= 8)
                levelCount[player.level]++;

            teamResources.food += player.inventory.food;
            teamResources.linemate += player.inventory.linemate;
            teamResources.deraumere += player.inventory.deraumere;
            teamResources.sibur += player.inventory.sibur;
            teamResources.mendiane += player.inventory.mendiane;
            teamResources.phiras += player.inventory.phiras;
            teamResources.thystame += player.inventory.thystame;
        }
    }

    const auto& eggs = _gameInfos->getEggs();
    int teamEggs = 0;
    for (const auto& egg : eggs) {
        if (egg.teamName == teamName)
            teamEggs++;
    }

    const auto& incantations = _gameInfos->getIncantations();
    int teamIncantations = 0;
    for (const auto& incantation : incantations) {
        for (const auto& player : teamPlayers) {
            if (player.x == incantation.x && player.y == incantation.y) {
                teamIncantations++;
                break;
            }
        }
    }

    float yPos = 18.0f;

    _teamDetailsContainer->addTextPercent(
        "total_players", 5.0f, yPos,
        "Total Players: " + std::to_string(totalPlayers),
        3.0f, {220, 220, 220, 255}
    );
    yPos += 4.0f;

    float averageLevel = totalPlayers > 0 ?
        static_cast<float>(totalLevels) / totalPlayers : 0.0f;
    _teamDetailsContainer->addTextPercent(
        "average_level", 5.0f, yPos,
        "Average Level: " + (totalPlayers > 0 ?
            std::to_string(averageLevel).substr(0, 4) : "0.0"),
        3.0f, {220, 220, 220, 255}
    );
    yPos += 4.0f;

    _teamDetailsContainer->addTextPercent(
        "level_distribution_title", 5.0f, yPos,
        "Level Distribution:",
        3.0f, {220, 220, 220, 255}
    );
    yPos += 3.0f;

    for (int level = 1; level <= 8; level++) {
        int r = std::min(255, 150 + level * 10);
        int g = std::min(255, 150 + level * 5);
        int b = std::max(0, 255 - level * 15);
        Color32 levelColor = levelCount[level] > 0 ?
            Color32{static_cast<unsigned char>(r), static_cast<unsigned char>(g),
                static_cast<unsigned char>(b), 255} :
            Color32{100, 100, 100, 255};

        _teamDetailsContainer->addTextPercent(
            "level_" + std::to_string(level), 10.0f, yPos,
            "Level " + std::to_string(level) + ": " + std::to_string(levelCount[level])
                + " player" + (levelCount[level] > 1 ? "s" : ""),
            2.5f, levelColor
        );
        yPos += 3.0f;
    }
    yPos += 1.0f;

    _teamDetailsContainer->addTextPercent(
        "max_level_status", 5.0f, yPos,
        levelCount[8] > 0 ?
            "MAX LEVEL REACHED!\n(" + std::to_string(levelCount[8]) + " at level 8)" :
            "No players at max\nlevel yet",
        3.0f, levelCount[8] > 0 ? Color32{255, 215, 0, 255} : Color32{220, 220, 220, 255}
    );
    yPos += 7.5f;

    _teamDetailsContainer->addTextPercent(
        "resources_title", 5.0f, yPos,
        "Team Resources:",
        3.0f, {220, 220, 220, 255}
    );
    yPos += 3.0f;

    std::vector<std::pair<std::string, int>> resources = {
        {"Food", teamResources.food},
        {"Linemate", teamResources.linemate},
        {"Deraumere", teamResources.deraumere},
        {"Sibur", teamResources.sibur},
        {"Mendiane", teamResources.mendiane},
        {"Phiras", teamResources.phiras},
        {"Thystame", teamResources.thystame}
    };

    for (const auto& res : resources) {
        Color32 resColor = res.second > 0 ? Color32{150, 255, 150, 255} :
            Color32{150, 150, 150, 255};
        _teamDetailsContainer->addTextPercent(
            "resource_" + res.first, 10.0f, yPos,
            res.first + ": " + std::to_string(res.second),
            2.2f, resColor
        );
        yPos += 2.5f;
    }
    yPos += 2.0f;

    _teamDetailsContainer->addTextPercent(
        "eggs_status", 5.0f, yPos,
        "Eggs Status:",
        3.0f, {220, 220, 220, 255}
    );
    yPos += 3.0f;

    _teamDetailsContainer->addTextPercent(
        "total_eggs", 10.0f, yPos,
        "Total Eggs: " + std::to_string(teamEggs),
        2.5f, teamEggs > 0 ? Color32{255, 200, 100, 255} : Color32{150, 150, 150, 255}
    );
    yPos += 4.5f;

    _teamDetailsContainer->addTextPercent(
        "incantations_status", 5.0f, yPos,
        "Active Incantations: " + std::to_string(teamIncantations),
        3.0f, Color32{220, 220, 220, 255}
    );
    yPos += 4.0f;

    _teamDetailsContainer->setVisible(true);
}

void HUD::hideTeamDetailsContainer()
{
    if (_teamDetailsContainer)
        _teamDetailsContainer->setVisible(false);
}
