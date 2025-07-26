/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AContainers
*/

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "IContainers.hpp"

struct RelativePosition {
    float xPercent;
    float yPercent;
    float widthPercent;
    float heightPercent;
};

class AContainers : public IContainers {
    public:
        AContainers(std::shared_ptr<IDisplay> display, float x, float y, float width,
            float height);

        virtual ~AContainers() = default;

        void setPosition(float x, float y) override;
        void setSize(float width, float height) override;
        FloatRect getBounds() const override;
        bool contains(float x, float y) const override;
        void setVisible(bool visible) override;
        bool isVisible() const override;

        void setRelativePosition(float xPercent, float yPercent, float widthPercent,
            float heightPercent);

        RelativePosition getRelativePosition() const;

        void updatePositionFromRelative();

        float getWidth() const;
        float getHeight() const;

    protected:
        std::shared_ptr<IDisplay> _display;
        FloatRect _bounds;
        RelativePosition _relativePos;
        Color32 _backgroundColor;
        bool _visible;
        bool _hasBackground;
};
