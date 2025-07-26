/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GuiObserver
*/

#include <string>
#include "GuiObserver.hpp"
#include "../Graphic/GUI.hpp"

GuiObserver::GuiObserver(std::shared_ptr<GUI> gui) : _gui(gui)
{
}

void GuiObserver::update()
{
    if (auto gui = _gui.lock()) {
        gui->refresh();
    }
}

void GuiObserver::onGameEvent(GameEventType eventType, const std::string& teamName)
{
    if (auto gui = _gui.lock()) {
        if (eventType == GameEventType::TEAM_WIN) {
            gui->handleVictory(teamName);
        }
    }
}
