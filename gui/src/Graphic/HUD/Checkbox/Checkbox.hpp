/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Checkbox
*/

#pragma once

#include <string>
#include <functional>
#include <memory>

#include "../UIElement/AUIElement.hpp"
#include "../../../Audio/IAudio.hpp"
#include "../../../IDisplay.hpp"

class Checkbox : public AUIElement {
    public:
        Checkbox(
            std::shared_ptr<IDisplay> display,
            std::shared_ptr<IAudio> audio,
            float x, float y,
            float width, float height,
            bool initialValue,
            std::function<void(bool)> callback
        );

        ~Checkbox() override = default;

        void draw() override;

        void update() override;

        void setCallback(std::function<void(bool)> callback);

        void setValue(bool value);

        bool getValue() const;

        void setColors(
            Color32 normalColor,
            Color32 hoverColor,
            Color32 pressedColor,
            Color32 checkColor
        );

        void setSize(float width, float height) override;

        bool containsCheckbox(float x, float y) const;

    private:
        bool _value;
        std::function<void(bool)> _callback;

        Color32 _normalColor;
        Color32 _hoverColor;
        Color32 _pressedColor;
        Color32 _checkColor;

        bool _isHovered;
        bool _isPressed;

        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<IAudio> _audio;

        float _checkboxSize;
};
