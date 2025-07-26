/*
** EPITECH PROJECT, 2025
** gui
** File description:
** SFMLAudio.hpp
*/

#ifndef SFMLAUDIO_HPP
#define SFMLAUDIO_HPP

#include <string>
#include <memory>
#include <map>
#include <SFML/Audio.hpp>
#include "../AAudio.hpp"


class SFMLAudio : public AAudio{
    private:
        std::map<std::string, std::unique_ptr<sf::Music>> _sounds;

    public:
        bool loadSound(const std::string& id, const std::string& filepath);

        void playSound(const std::string& id, float volume);
        void stopSound(const std::string& id);
        bool isSoundPlaying(const std::string& id) const;

        void setSoundLooping(const std::string& id, bool looping);

        void setSoundVolume(const std::string& id, float volume);

        SFMLAudio();
        ~SFMLAudio();
};



#endif  // SFMLAUDIO_HPP
