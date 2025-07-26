/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Audio
*/

#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "IAudio.hpp"

class AAudio : public IAudio {
    protected:
        std::vector<std::string> _musicId = {"main_theme", "main_theme2"};
        std::vector<std::string> _sfxId = {"click", "clickPlayer", "collect",
                                            "win", "loose", "zap", "splash_screen"};
        float _levelSFX = 75.f;
        float _levelMusic = 50.f;
        int _themeIndex = 0;

    public:
        AAudio() = default;
        ~AAudio() = default;

        float getSFXVolumeLevel();
        float getMusicVolumeLevel();

        void setSFXVolumeLevel(float);
        void setMusicVolumeLevel(float);

        void playMainTheme(float volume);
        void playNextTheme(float volume);
};

#endif /* !AUDIO_HPP_ */
