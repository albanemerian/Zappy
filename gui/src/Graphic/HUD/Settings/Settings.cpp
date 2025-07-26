/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Settings
*/

#include <iostream>
#include <string>
#include <memory>
#include <cctype>
#include <vector>
#include "Settings.hpp"
#include "Utils/HelpText.hpp"

void Settings::show()
{
    this->_visible = true;
}

void Settings::hide()
{
    this->_visible = false;
}

bool Settings::isVisible() const
{
    return this->_visible;
}

void Settings::update()
{
    if (!this->_visible || !this->_settingsContainer)
        return;

    this->_settingsContainer->update();
    if (this->_audio->getMusicVolumeLevel() != this->_musicLevel)
        this->_audio->setMusicVolumeLevel(this->_musicLevel);
    if (this->_audio->getSFXVolumeLevel() != this->_sfxLevel)
        this->_audio->setSFXVolumeLevel(this->_sfxLevel);
    if (this->_camera->getCameraMovingSpeed() != this->_cameraMovingSpeed)
        this->_camera->setCameraMovingSpeed(this->_cameraMovingSpeed);
    if (this->_camera->getCameraRotaSpeed() != this->_cameraRotaSpeed)
        this->_camera->setCameraRotaSpeed(this->_cameraRotaSpeed);
    if (this->_camera->getCameraZoomSpeed() != this->_cameraZoomSpeed)
        this->_camera->setCameraZoomSpeed(this->_cameraZoomSpeed);
}

void Settings::draw()
{
    if (!this->_visible || !this->_settingsContainer)
        return;

    this->_settingsContainer->draw();
}

void Settings::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    if (!this->_settingsContainer)
        return;

    this->_settingsContainer->handleResize(oldWidth, oldHeight, newWidth, newHeight);
}


Settings::Settings(
    std::shared_ptr<IDisplay> display,
    std::shared_ptr<IAudio> audio,
    std::shared_ptr<CameraManager> camera,
    std::shared_ptr<GameInfos> gameInfos
):
    _display(display), _audio(audio), _camera(camera), _gameInfos(gameInfos)
{
    if (!_display)
        return;
    this->_cameraMovingSpeed = this->_camera->getCameraMovingSpeed();
    this->_cameraRotaSpeed = this->_camera->getCameraRotaSpeed();
    this->_cameraZoomSpeed = this->_camera->getCameraZoomSpeed();
    this->_settingsContainer = std::make_shared<Containers>(
        this->_display,
        this->_audio,
        15.0f, 0.0f,
        30.0f, 30.0f,
        Color32{50, 50, 50, 240}
    );
    this->_musicLevel = this->_audio->getMusicVolumeLevel();
    this->_sfxLevel = this->_audio->getSFXVolumeLevel();

    this->_settingsContainer->setRelativePosition(
        15.0f, 0.0f,
        60.0f, 40.0f
    );

    this->_settingsContainer->addTextPercent(
        "settings_title",
        10.0f, 4.0f,
        "Settings",
        10.0f,
        {255, 255, 255, 255}
    );

    this->_settingsContainer->addButtonPercent(
        "settings_close_button",
        87.0f, 3.0f,
        10.0f, 8.0f,
        "X",
        [this]() {
            this->hide();
        },
        {200, 60, 60, 255},
        {255, 100, 100, 255},
        {160, 40, 40, 255},
        {255, 255, 255, 255}
    );

    this->_settingsContainer->addTextPercent(
        "settings_separator",
        0.0f, 16.0f,
        std::string(350, '-'),
        1.0f,
        {150, 150, 150, 200}
    );

    float yPos = 22.0f;

    this->_settingsContainer->addSliderPercent(
        "slider_music",
        5.f, yPos, 45.f, 16.f,
        0.f, 100.f, this->_audio->getMusicVolumeLevel(), "Volume Music:",
        [this](float nbr) {
            this->_musicLevel = nbr;
        }
    );
    yPos += 16.0f;

    this->_settingsContainer->addSliderPercent(
        "slider_click",
        5.f, yPos, 45.f, 16.f,
        0.f, 100.f, this->_audio->getSFXVolumeLevel(), "Volume SFX:",
        [this](float nbr) {
            this->_sfxLevel = nbr;
        }
    );

    this->_settingsContainer->addImageButtonPercent(
        "test_music_button",
        33.0f, 42.f,
        25.0f, 25.0f,
        "gui/assets/sprite/hp_logo.png",
        [this]() {
        }
    );
    yPos += 16.0f;

    this->_settingsContainer->addSliderPercent(
        "slider_camera_speed_moving",
        5.f, yPos, 45.f, 16.f,
        0.1f, 250.f, this->_cameraMovingSpeed, "Camera Moving Speed:",
        [this](float nbr) {
            this->_cameraMovingSpeed = nbr;
        }
    );
    yPos += 16.0f;

    this->_settingsContainer->addSliderPercent(
        "slider_camera_speed_rota",
        5.f, yPos, 45.f, 16.f,
        0.1f, 20.f, this->_cameraRotaSpeed, "Camera Rota Speed:",
        [this](float nbr) {
            this->_cameraRotaSpeed = nbr;
        }
    );
    yPos += 16.0f;

    this->_settingsContainer->addSliderPercent(
        "slider_camera_speed_zoom",
        5.f, yPos, 45.f, 16.f,
        50.f, 500.f, this->_cameraZoomSpeed, "Camera Zoom Speed:",
        [this](float nbr) {
            this->_cameraZoomSpeed = nbr;
        }
    );
    yPos += 20.0f;

    float rightColumnX = 52.0f;
    float rightColumnY = 22.0f;

    this->_settingsContainer->addTextPercent(
        "objects_title",
        rightColumnX, rightColumnY,
        "Object Visibility:",
        6.0f,
        {255, 255, 255, 255}
    );
    rightColumnY += 8.0f;

    this->_settingsContainer->addTextPercent(
        "objects_separator",
        rightColumnX - 5.0f, rightColumnY,
        std::string(125, '-'),
        1.5f,
        {120, 120, 120, 150}
    );
    rightColumnY += 6.0f;

    float firstColumnX = rightColumnX - 3.0f;
    float secondColumnX = rightColumnX + 20.0f;
    float currentY = rightColumnY;

    std::vector<std::pair<std::string, std::string>> allItems = {
        {"players", "Players"},
        {"eggs", "Eggs"},
        {"food", "Food"},
        {"linemate", "Linemate"},
        {"deraumere", "Deraumere"},
        {"sibur", "Sibur"},
        {"mendiane", "Mendiane"},
        {"phiras", "Phiras"},
        {"thystame", "Thystame"}
    };

    for (size_t i = 0; i < allItems.size(); ++i) {
        const auto& item = allItems[i];
        float currentX = (i % 2 == 0) ? firstColumnX : secondColumnX;

        if (i % 2 == 1 && i > 0) {
        } else if (i > 1) {
            currentY += 10.0f;
        }

        this->_settingsContainer->addCheckboxPercent(
            item.first + "_checkbox", currentX, currentY,
            25.0f, 8.0f, true,
            [this, itemKey = item.first](bool checked) {
                this->_gameInfos->setObjectVisibility(itemKey, checked);
            }
        );

        this->_settingsContainer->addTextPercent(
            item.first + "_title", currentX + 4.0f, currentY + 2.0f,
            item.second, 5.5f, {255, 255, 255, 255}
        );
    }

    this->_visible = false;
}

Settings::~Settings()
{
}

bool Settings::containsPoint(float x, float y) const
{
    if (!this->_settingsContainer || !this->_visible)
        return false;

    return this->_settingsContainer->contains(x, y);
}
