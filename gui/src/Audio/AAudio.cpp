/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Audio
*/

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <random>
#include <chrono>
#include <algorithm>
#include <SFML/Audio.hpp>
#include "AAudio.hpp"

float AAudio::getSFXVolumeLevel() {
    return this->_levelSFX;
}

float AAudio::getMusicVolumeLevel() {
    return this->_levelMusic;
}

void AAudio::setSFXVolumeLevel(float level) {
    this->_levelSFX = level;
    for (const auto &s : this->_sfxId)
        this->setSoundVolume(s, this->_levelSFX);
}

void AAudio::setMusicVolumeLevel(float level) {
    this->_levelMusic = level;
    for (const auto &s : this->_musicId)
        this->setSoundVolume(s, this->_levelMusic);
}

void AAudio::playMainTheme(float volume)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::uniform_int_distribution<size_t> dist(0, _musicId.size() - 1);
    _themeIndex = dist(rng);
    const std::string& themeId = _musicId[_themeIndex];

    this->setSoundLooping(themeId, false);
    this->playSound(themeId, volume);
}

void AAudio::playNextTheme(float volume)
{
    if (isSoundPlaying(_musicId[_themeIndex]))
        return;

    _themeIndex = (_themeIndex + 1) % _musicId.size();
    const std::string& nextThemeId = _musicId[_themeIndex];

    this->setSoundLooping(nextThemeId, false);
    this->playSound(nextThemeId, volume);
}
