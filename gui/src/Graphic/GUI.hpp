/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GUI
*/

#ifndef GUI_HPP_
#define GUI_HPP_

#include <memory>
#include <string>
#include <utility>
#include <mutex>
#include "../Game/GameInfos.hpp"
#include "Map.hpp"
#include "HUD/HUD.hpp"
#include "SplashScreen.hpp"
#include "../Audio/IAudio.hpp"
#include "../Utils/Constants.hpp"
#include "Camera/CameraManager.hpp"
#include "../IDisplay.hpp"
#include "../DLLoader/DLLoader.hpp"
#include "DLLoader/LoaderType.hpp"

class GUI {
    public:
        GUI(std::shared_ptr<GameInfos> gameInfos,
            const std::string &libGraphicPath,
            const std::string &libAudioPath);
        ~GUI();

        void run();
        void stop();
        void refresh();
        void handleVictory(const std::string &teamName);

        int getWindowWidth() const;
        int getWindowHeight() const;
        void setWindowWidth(int width);
        void setWindowHeight(int height);

        void switchCameraMode(zappy::gui::CameraMode mode);
        void switchCameraModeNext();
        void setPlayerToFollow(int playerId);
        int getPlayerToFollow() const;
        bool selectFirstAvailablePlayer();
        void switchToNextPlayer();
        void switchToPreviousPlayer();

    private:
        static int _getRandomTime(int min, int max);
        void updateCamera();
        virtual void update();
        virtual void draw();
        virtual bool isRunning();
        bool playerExists(int playerId) const;
        void drawSplashFrame();

        void initModels();
        void initPlayers();
        void handlePlayerClicks();
        int getPlayerUnderMouse() const;
        BoundingBox3D getPlayerBoundingBox(const zappy::structs::Player& player) const;

        void handleTileClicks();
        std::pair<int, int> getTileUnderMouse() const;
        BoundingBox3D getTileBoundingBox(int x, int y) const;

        std::string _currentLibLoaded;
        bool _isRunning;

        DLLoader<std::shared_ptr<IDisplay>> _dlLoaderGraphic;
        DLLoader<std::shared_ptr<IDisplay>> _dlLoaderAudio;
        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<GameInfos> _gameInfos;
        std::unique_ptr<Map> _map;
        std::unique_ptr<HUD> _hud;
        std::unique_ptr<SplashScreen> _splashScreen;
        std::shared_ptr<IAudio> _audio;
        std::shared_ptr<CameraManager> _cameraManager;

        int _windowWidth;
        int _windowHeight;

        zappy::gui::CameraMode _cameraMode;
        bool _isHUDVisible = true;
        bool _backgroundLoaded;
        bool _skyboxLoaded;
        int _hoveredPlayerId;
        std::pair<int, int> _selectedTile;

        bool _performanceMode = false;
        bool _showingSplashScreen = true;
        bool _loadingComplete = false;

        mutable std::mutex _playerMutex;
};

#endif /* !GUI_HPP_ */
