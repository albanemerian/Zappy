/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** IObserver
*/

#ifndef IOBSERVER_HPP_
#define IOBSERVER_HPP_

#include <string>

enum class GameEventType {
    STATE_CHANGED,
    TEAM_WIN,
    TEAM_DEFEAT
};

class IObserver {
    public:
        virtual ~IObserver() = default;
        virtual void update() = 0;
        virtual void onGameEvent(GameEventType eventType, const std::string& teamName) {
            (void)eventType;
            (void)teamName;
        }
};

#endif /* !IOBSERVER_HPP_ */
