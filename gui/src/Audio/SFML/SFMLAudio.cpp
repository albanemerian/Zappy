/*
** EPITECH PROJECT, 2025
** gui
** File description:
** SFMLAudio.cpp
*/

#include "SFMLAudio.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <memory>

SFMLAudio::SFMLAudio()
{
    this->SFMLAudio::loadSound("main_theme", "gui/assets/sounds/main_theme.wav");
    this->SFMLAudio::loadSound("main_theme2", "gui/assets/sounds/main_theme2.wav");
    this->SFMLAudio::loadSound("click", "gui/assets/sounds/click.wav");
    this->SFMLAudio::loadSound("clickPlayer", "gui/assets/sounds/playerClick.wav");
    this->SFMLAudio::loadSound("collect", "gui/assets/sounds/collect.wav");
    this->SFMLAudio::loadSound("win", "gui/assets/sounds/win.wav");
    this->SFMLAudio::loadSound("loose", "gui/assets/sounds/loose.wav");
    this->SFMLAudio::loadSound("zap", "gui/assets/sounds/zap.wav");
    this->SFMLAudio::loadSound("splash_screen", "gui/assets/sounds/splash_screen.wav");
}

SFMLAudio::~SFMLAudio() {
    for (auto &sound : _sounds) {
        if (sound.second) {
            sound.second->stop();
        }
    }
}

bool SFMLAudio::loadSound(const std::string& id, const std::string& filepath)
{
    if (_sounds.find(id) != _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' already exists." << std::endl;
        return false;
    }

    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filepath)) {
        std::cerr << "Failed to load sound from '" << filepath << "'" << std::endl;
        return false;
    }

    _sounds[id] = std::move(music);
    return true;
}

void SFMLAudio::playSound(const std::string& id, float volume)
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return;
    }
    it->second->stop();
    auto musicIt = std::find(this->_musicId.begin(), this->_musicId.end(), id);
    if (musicIt != this->_musicId.end())
        it->second->setVolume(this->_levelMusic);
    else {
        auto sfxIt = std::find(this->_sfxId.begin(), this->_sfxId.end(), id);
        if (sfxIt != this->_sfxId.end())
            it->second->setVolume(this->_levelSFX);
        else {
            it->second->setVolume(volume);
        }
    }
    it->second->play();
}

void SFMLAudio::stopSound(const std::string& id)
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return;
    }

    it->second->stop();
}

bool SFMLAudio::isSoundPlaying(const std::string& id) const
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return false;
    }

    return it->second->getStatus() == sf::Music::Playing;
}

void SFMLAudio::setSoundLooping(const std::string& id, bool looping)
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return;
    }

    it->second->setLoop(looping);
}

void SFMLAudio::setSoundVolume(const std::string& id, float volume)
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return;
    }

    it->second->setVolume(volume);
}
