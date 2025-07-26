/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** SplashScreen
*/

#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <string>
#include "SplashScreen.hpp"
#include "../Utils/Constants.hpp"

SplashScreen::SplashScreen(std::shared_ptr<IDisplay> display)
    : _display(display), _currentTime(0.0f),
      _fadeAlpha(1.0f), _logoScale(1.0f), _loadingProgress(0.0f),
      _loadingText("Loading..."), _finished(false), _logoLoaded(false)
{
    auto screenSize = _display->getScreenSize();
    _windowWidth = screenSize.x;
    _windowHeight = screenSize.y;

    _logoLoaded = _display->loadTexture("zappyLogo", "gui/assets/img/zappyLogo.png");
    if (!_logoLoaded) {
        std::cout << "[WARNING] Failed to load Zappy logo: gui/assets/img/zappyLogo.png"
            << std::endl;
    }
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::show()
{
    _currentTime = 0.0f;
    _finished = false;
}

void SplashScreen::update(float deltaTime)
{
    if (_finished)
        return;

    _currentTime += deltaTime;
    _fadeAlpha = 1.0f;
    _logoScale = 1.0f;
}

void SplashScreen::draw()
{
    if (_finished)
        return;

    _display->beginDrawing();
    _display->clearBackground(Color32{0, 0, 0, 255});
    _drawBackground();
    _drawZappyLogo();
    _drawLoadingBar();
    _drawLoadingText();
    _display->endDrawing();
}

void SplashScreen::_drawBackground()
{
    Color32 topColor = {20, 30, 50, 255};
    Color32 bottomColor = {10, 20, 40, 255};
    int steps = 20;
    float stepHeight = static_cast<float>(_windowHeight) / steps;

    for (int i = 0; i < steps; ++i) {
        float ratio = static_cast<float>(i) / steps;

        Color32 currentColor = {
            static_cast<unsigned char>(topColor.r + (bottomColor.r - topColor.r) * ratio),
            static_cast<unsigned char>(topColor.g + (bottomColor.g - topColor.g) * ratio),
            static_cast<unsigned char>(topColor.b + (bottomColor.b - topColor.b) * ratio),
            255
        };

        FloatRect rect = {0, i * stepHeight, static_cast<float>(_windowWidth),
            stepHeight + 1};
        _display->drawRectangleRec(rect, currentColor);
    }
}

void SplashScreen::_drawZappyLogo()
{
    float centerX = _windowWidth / 2.0f;
    float centerY = _windowHeight / 2.0f - 100;

    if (_logoLoaded) {
        float logoWidth = 180.0f * _logoScale;
        float logoHeight = 200.0f * _logoScale;

        float positionX = centerX - logoWidth / 2.0f;
        float positionY = centerY - logoHeight / 2.0f;

        Color32 imageColor = {255, 255, 255, static_cast<unsigned char>(255 * _fadeAlpha)};
        _display->drawTextureScaled("zappyLogo", positionX, positionY, logoWidth,
            logoHeight, imageColor);

        std::string title = zappy::gui::SPLASH_TITLE;
        float titleY = centerY + logoHeight / 2.0f + 20;
        float fontSize = 42.0f * _logoScale;

        Color32 textColor = {255, 255, 255, static_cast<unsigned char>(255 * _fadeAlpha)};
        float textWidth = _display->measureText(title, fontSize);
        float titleX = centerX - textWidth / 2.0f;

        _display->drawText(title, titleX, titleY, fontSize, textColor);

        std::string subtitle = zappy::gui::SPLASH_SUBTITLE;
        float subtitleY = titleY + 50;
        float subtitleFontSize = 20.0f * _logoScale;
        Color32 subtitleColor = {180, 220, 255, static_cast<unsigned char>(200 * _fadeAlpha)};

        float subtitleWidth = _display->measureText(subtitle, subtitleFontSize);
        float subtitleX = centerX - subtitleWidth / 2.0f;

        _display->drawText(subtitle, subtitleX, subtitleY, subtitleFontSize, subtitleColor);
    } else {
        float logoSize = 60.0f * _logoScale;
        Color32 primaryColor = {120, 180, 255, static_cast<unsigned char>(220 * _fadeAlpha)};

        FloatRect topBar = {centerX - logoSize, centerY - logoSize, logoSize * 2,
            logoSize * 0.3f};
        _display->drawRectangleRec(topBar, primaryColor);

        FloatRect bottomBar = {centerX - logoSize, centerY + logoSize * 0.7f, logoSize * 2,
            logoSize * 0.3f};
        _display->drawRectangleRec(bottomBar, primaryColor);

        for (int i = 0; i < 20; ++i) {
            float t = static_cast<float>(i) / 19.0f;
            float x = centerX + logoSize - (logoSize * 2 * t);
            float y = centerY - logoSize + (logoSize * 2 * t);
            FloatRect smallRect = {x - 2, y - 2, 4, 4};
            _display->drawRectangleRec(smallRect, primaryColor);
        }

        std::string title = zappy::gui::SPLASH_TITLE;
        float titleY = centerY + logoSize + 60;
        float fontSize = 42.0f * _logoScale;

        Color32 textColor = {255, 255, 255, static_cast<unsigned char>(255 * _fadeAlpha)};
        float textWidth = _display->measureText(title, fontSize);
        float titleX = centerX - textWidth / 2.0f;

        _display->drawText(title, titleX, titleY, fontSize, textColor);

        std::string subtitle = zappy::gui::SPLASH_SUBTITLE;
        float subtitleY = titleY + 50;
        float subtitleFontSize = 20.0f * _logoScale;
        Color32 subtitleColor = {180, 220, 255, static_cast<unsigned char>(200 * _fadeAlpha)};

        float subtitleWidth = _display->measureText(subtitle, subtitleFontSize);
        float subtitleX = centerX - subtitleWidth / 2.0f;

        _display->drawText(subtitle, subtitleX, subtitleY, subtitleFontSize, subtitleColor);
    }
}

void SplashScreen::_drawLoadingBar()
{
    float centerX = _windowWidth / 2.0f;
    float barY = _windowHeight / 2.0f + 120;
    float barWidth = 400.0f;
    float barHeight = 24.0f;

    FloatRect bgRect = {centerX - barWidth / 2.0f, barY, barWidth, barHeight};
    Color32 bgColor = {40, 50, 60, static_cast<unsigned char>(180 * _fadeAlpha)};
    _display->drawRectangleRec(bgRect, bgColor);

    float progressWidth = barWidth * _loadingProgress;
    if (progressWidth > 0) {
        FloatRect progressRect = {centerX - barWidth / 2.0f, barY + 2, progressWidth - 4,
            barHeight - 4};
        Color32 progressColor = {100, 200, 120, static_cast<unsigned char>(220 * _fadeAlpha)};
        _display->drawRectangleRec(progressRect, progressColor);
    }

    Color32 borderColor = {150, 200, 255, static_cast<unsigned char>(150 * _fadeAlpha)};
    float borderThickness = 2.0f;

    FloatRect topBorder = {centerX - barWidth / 2.0f, barY, barWidth, borderThickness};
    FloatRect bottomBorder = {centerX - barWidth / 2.0f, barY + barHeight - borderThickness,
        barWidth, borderThickness};
    FloatRect leftBorder = {centerX - barWidth / 2.0f, barY, borderThickness, barHeight};
    FloatRect rightBorder = {centerX + barWidth / 2.0f - borderThickness, barY,
        borderThickness, barHeight};

    _display->drawRectangleRec(topBorder, borderColor);
    _display->drawRectangleRec(bottomBorder, borderColor);
    _display->drawRectangleRec(leftBorder, borderColor);
    _display->drawRectangleRec(rightBorder, borderColor);
}

void SplashScreen::_drawLoadingText()
{
    float centerX = _windowWidth / 2.0f;
    float textY = _windowHeight / 2.0f + 170;
    float fontSize = 20.0f;

    Color32 textColor = {220, 240, 255, static_cast<unsigned char>(255 * _fadeAlpha)};

    float textWidth = _display->measureText(_loadingText, fontSize);
    float textX = centerX - textWidth / 2.0f;

    _display->drawText(_loadingText, textX, textY, fontSize, textColor);

    std::string progressText = std::to_string(static_cast<int>(_loadingProgress * 100)) + "%";
    float progressY = textY + 35;
    float progressFontSize = 24.0f;

    Color32 progressColor = {150, 255, 150, static_cast<unsigned char>(255 * _fadeAlpha)};
    float progressWidth = _display->measureText(progressText, progressFontSize);
    float progressX = centerX - progressWidth / 2.0f;

    _display->drawText(progressText, progressX, progressY, progressFontSize, progressColor);
}

void SplashScreen::setLoadingProgress(float progress)
{
    _loadingProgress = std::clamp(progress, 0.0f, 1.0f);
}

void SplashScreen::setLoadingText(const std::string& text)
{
    _loadingText = text;
}

void SplashScreen::finish()
{
    _finished = true;
}
