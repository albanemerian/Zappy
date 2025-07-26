/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Button
*/

#pragma once

#include <string>
#include <functional>
#include <memory>

#include "../UIElement/AUIElement.hpp"
#include "../../../Audio/IAudio.hpp"
#include "../../../IDisplay.hpp"

class Button : public AUIElement {
    public:
        Button(
            std::shared_ptr<IDisplay> display,
            std::shared_ptr<IAudio> audio,
            float x, float y,
            float width, float height,
            const std::string& text,
            std::function<void()> callback
        );

        ~Button() override = default;

        void draw() override;

        void update() override;

        void setText(const std::string& text);

        std::string getText() const;

        void setCallback(std::function<void()> callback);

        void setColors(
            Color32 normal,
            Color32 hover,
            Color32 pressed,
            Color32 textColor
        );

        void setSize(float width, float height) override;

    private:
        std::string _text;
        std::function<void()> _callback;

        Color32 _normalColor;
        Color32 _hoverColor;
        Color32 _pressedColor;
        Color32 _textColor;

        bool _isHovered;
        bool _isPressed;

        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<IAudio> _audio;
};
