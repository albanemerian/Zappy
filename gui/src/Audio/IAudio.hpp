/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** IAudio
*/

#ifndef IAUDIO_HPP_
#define IAUDIO_HPP_

#include <string>

class IAudio {
    public:
        virtual ~IAudio() = default;

        virtual float getSFXVolumeLevel() = 0;
        virtual float getMusicVolumeLevel() = 0;

        virtual void setSFXVolumeLevel(float) = 0;
        virtual void setMusicVolumeLevel(float) = 0;

        virtual bool loadSound(const std::string& id, const std::string& filepath) = 0;

        virtual void playMainTheme(float volume) = 0;
        virtual void playNextTheme(float volume) = 0;

        virtual void playSound(const std::string& id, float volume) = 0;
        virtual void stopSound(const std::string& id) = 0;
        virtual bool isSoundPlaying(const std::string& id) const = 0;

        virtual void setSoundLooping(const std::string& id, bool looping) = 0;
        virtual void setSoundVolume(const std::string& id, float volume) = 0;
};

#endif /* !IAUDIO_HPP_ */
