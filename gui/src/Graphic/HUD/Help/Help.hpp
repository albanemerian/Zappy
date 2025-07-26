/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Help
*/

#pragma once

#include <memory>
#include <string>
#include "../Containers/Containers.hpp"
#include "../../../IDisplay.hpp"
#include "../../../Audio/IAudio.hpp"

class Help {
    public:
        Help(std::shared_ptr<IDisplay> display, std::shared_ptr<IAudio> audio);

        ~Help() = default;

        void show();

        void hide();

        bool isVisible() const;

        bool containsPoint(float x, float y) const;

        void update();

        void draw();

        void handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight);

    private:
        void initHelpContainer();

        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<IAudio> _audio;
        std::shared_ptr<Containers> _helpContainer;
        bool _visible;
};
