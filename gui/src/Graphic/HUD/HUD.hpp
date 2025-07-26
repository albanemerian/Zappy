/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HUD
*/

#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <utility>
#include <functional>
#include <chrono>
#include "Containers/Containers.hpp"
#include "../../Game/GameInfos.hpp"
#include "../../Audio/IAudio.hpp"
#include "../../Utils/Constants.hpp"
#include "Help/Help.hpp"
#include "Settings/Settings.hpp"
#include "../../IDisplay.hpp"
#include "../../Observer/IObserver.hpp"
#include "Graphic/Camera/CameraManager.hpp"

class HUD : public IObserver {
    public:
        HUD(std::shared_ptr<IDisplay> display, std::shared_ptr<GameInfos> gameInfos,
            std::shared_ptr<IAudio> audio,
            std::shared_ptr<CameraManager>,
            bool performanceMode,
            std::function<void()> resetCameraFunc = nullptr);

        ~HUD();

        void draw();

        std::shared_ptr<Containers> addContainer(
            const std::string& id,
            float x, float y,
            float width, float height,
            Color32 backgroundColor = {40, 40, 40, 200}
        );

        std::shared_ptr<Containers> getContainer(const std::string& id) const;

        bool removeContainer(const std::string& id);

        void handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight);

        void clearAllContainers();

        void initDefaultLayout(float sideWidthPercent = 15.0f,
            float bottomHeightPercent = 20.0f);

        std::shared_ptr<Containers> getSideContainer() const;

        std::shared_ptr<Containers> getBottomContainer() const;

        std::shared_ptr<Containers> getSquareContainer() const;

        std::shared_ptr<Containers> getTpsContainer() const;

        std::shared_ptr<Containers> getSecurityContainer() const;

        std::shared_ptr<Containers> getServerMessagesContainer() const;

        std::shared_ptr<Containers> getMapInfoContainer() const;

        void initExitButton();

        void initSettingsButton();

        void initHelpButton();

        void initCameraResetButton();

        void initTeamPlayersDisplay(std::shared_ptr<GameInfos> gameInfos);

        void updateTeamPlayersDisplay(std::shared_ptr<GameInfos> gameInfos);

        void initTpsSlider(std::shared_ptr<GameInfos> gameInfos,
            std::shared_ptr<IDisplay> raylib, std::shared_ptr<IAudio> audio);

        void updateTpsSlider(std::shared_ptr<GameInfos> gameInfos);

        void initServerMessagesDisplay(std::shared_ptr<GameInfos> gameInfos);

        void updateServerMessagesDisplay(std::shared_ptr<GameInfos> gameInfos);

        void initPlayerInventoryDisplay(int playerId);

        void updatePlayerInventoryDisplay(int playerId, zappy::gui::CameraMode cameraMode);

        void updateHelpInformationHUD(zappy::gui::CameraMode cameraMode);

        void clearPlayerInventoryElements();

        void setSelectedTile(int x, int y);

        void initTileResourceDisplay();

        void updateTileResourceDisplay(int x, int y);

        void clearTileResourceElements();

        void initFpsDisplay();

        void updateFpsDisplay();

        void initMapInfoDisplay();

        void initMapInfoButton();

        void updateMapInfoDisplay();

        zappy::structs::Player getPlayerById(int playerId) const;

        bool isPlayerInIncantation(int playerId) const;

        void setResetCameraCallback(std::function<void()> resetFunc);

        void displayWinMessage(const std::string& teamName);

        void update() override;
        void onGameEvent(GameEventType eventType, const std::string& teamName) override;

        bool isMouseOverHUD() const;

    private:
        void _initHelpInformation();

        std::string _camModeToText(zappy::gui::CameraMode, bool isGamePadAvailable);

        std::string _camKeyHelp(zappy::gui::CameraMode, bool isGamePadAvailable);

        std::string _mapGlobalInfo(std::shared_ptr<GameInfos> gameInfos);

        std::shared_ptr<Containers> createSquareContainer(float squareSize,
            float sideWidthPercent);

        std::shared_ptr<Containers> createSideContainer(
            float sideYStart,
            float sideWidth,
            float sideHeight,
            float sideWidthPercent,
            float bottomHeightPercent);

        std::shared_ptr<Containers> createBottomContainer(
            int screenWidth,
            int screenHeight,
            float bottomHeight,
            float bottomHeightPercent);

        std::shared_ptr<Containers> createTpsContainer(
            int screenWidth,
            int screenHeight,
            float bottomHeight,
            float bottomHeightPercent);

        std::shared_ptr<Containers> createSecurityContainer(
            int screenWidth,
            int screenHeight,
            float bottomHeight,
            float bottomHeightPercent);

        std::shared_ptr<Containers> createServerMessagesContainer(
            int screenWidth,
            int screenHeight,
            float bottomHeight,
            float bottomHeightPercent);

        std::shared_ptr<Containers> createMapInfoContainer();

        void updateElementPositions(
            std::shared_ptr<Containers> container,
            const std::unordered_map<std::string, float>& initialYPositions,
            float offset);

        std::pair<float, float> calculateContentMetrics(
            std::shared_ptr<Containers> container,
            const std::unordered_map<std::string, float>& initialYPositions);

        void clearTeamDisplayElements(std::shared_ptr<Containers> container);

        std::vector<int> getTeamPlayerNumbers(const std::string& teamName,
            const std::vector<zappy::structs::Player>& players);

        std::string createPlayerListText(const std::vector<int>& playerNumbers);

        void addPlayerListText(std::shared_ptr<Containers> container,
                            const std::string& teamId,
                            float yPos, const std::vector<int>& playerNumbers);

        void addIncrementDecrementButtons(std::shared_ptr<Containers> container, int playerId);

        void updateTeamHoverDetection();
        void createTeamDetailsContainer();
        void showTeamDetailsContainer(const std::string& teamName);
        void hideTeamDetailsContainer();

        std::unordered_map<std::string, std::shared_ptr<Containers>> _containers;
        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<IAudio> _audio;
        std::shared_ptr<CameraManager> _camera;
        bool _performanceMode;
        std::shared_ptr<Help> _help;
        std::shared_ptr<Settings> _settings;
        std::function<void()> _resetCameraFunc;
        bool _showVictoryMessage;
        std::string _winningTeam;
        Color32 _victoryColor;
        std::pair<int, int> _selectedTile;

        std::string _hoveredTeam;
        std::shared_ptr<Containers> _teamDetailsContainer;
        bool _mapInfoButtonHovered;
};
