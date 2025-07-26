/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** IUIElement
*/

#pragma once

#include "../../../IDisplay.hpp"

class IUIElement {
    public:
        virtual ~IUIElement() = default;

        virtual void draw() = 0;

        virtual void update() = 0;

        virtual void setPosition(float x, float y) = 0;

        virtual void setSize(float width, float height) = 0;

        virtual FloatRect getBounds() const = 0;

        virtual bool contains(float x, float y) const = 0;

        virtual void setVisible(bool visible) = 0;

        virtual bool isVisible() const = 0;
};
