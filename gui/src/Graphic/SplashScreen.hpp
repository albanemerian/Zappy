/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** SplashScreen
*/

#ifndef SPLASHSCREEN_HPP_
#define SPLASHSCREEN_HPP_

#include <memory>
#include <string>
#include <vector>
#include "../IDisplay.hpp"

class SplashScreen {
    public:
        SplashScreen(std::shared_ptr<IDisplay> display);
        ~SplashScreen();

        void show();
        void update(float deltaTime);
        void draw();
        void setLoadingProgress(float progress);
        void setLoadingText(const std::string& text);
        void finish();

    private:
        void _drawBackground();
        void _drawLoadingBar();
        void _drawLoadingText();
        void _drawZappyLogo();

        std::shared_ptr<IDisplay> _display;
        float _currentTime;
        float _fadeAlpha;
        float _logoScale;
        float _loadingProgress;
        std::string _loadingText;
        bool _finished;
        bool _logoLoaded;

        int _windowWidth;
        int _windowHeight;
};

#endif /* !SPLASHSCREEN_HPP_ */
