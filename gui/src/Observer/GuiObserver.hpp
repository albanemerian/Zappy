/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GuiObserver
*/

#ifndef GUIOBSERVER_HPP_
#define GUIOBSERVER_HPP_

#include <memory>
#include <string>
#include "IObserver.hpp"

class GUI;

class GuiObserver : public IObserver {
    public:
        GuiObserver(std::shared_ptr<GUI> gui);
        virtual ~GuiObserver() = default;

        void update() override;
        void onGameEvent(GameEventType eventType, const std::string& teamName) override;

    private:
        std::weak_ptr<GUI> _gui;
};

#endif /* !GUIOBSERVER_HPP_ */
