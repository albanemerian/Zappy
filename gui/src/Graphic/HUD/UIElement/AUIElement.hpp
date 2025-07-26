/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AUIElement
*/

#pragma once

#include <memory>
#include "IUIElement.hpp"

struct UIRelativePosition {
    float xPercent;
    float yPercent;
    float widthPercent;
    float heightPercent;
};

class AUIElement : public IUIElement {
    public:
        AUIElement(std::shared_ptr<IDisplay> display, float x, float y, float width,
            float height);

        virtual ~AUIElement() = default;

        // IUIElement implementation
        void setPosition(float x, float y) override;
        FloatRect getBounds() const override;
        bool contains(float x, float y) const override;
        void setVisible(bool visible) override;
        bool isVisible() const override;

        virtual void setSize(float width, float height);

        void setRelativePosition(float xPercent, float yPercent, float widthPercent,
            float heightPercent);

        UIRelativePosition getRelativePosition() const;

    protected:
        std::shared_ptr<IDisplay> _display;
        FloatRect _bounds;
        UIRelativePosition _relativePos;
        bool _visible;
};
