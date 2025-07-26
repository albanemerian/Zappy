/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Text
*/

#pragma once

#include <memory>
#include <string>

#include "../UIElement/AUIElement.hpp"
#include "../../../IDisplay.hpp"

class Text : public AUIElement {
    public:
        Text(
            std::shared_ptr<IDisplay> raylib,
            float x, float y,
            const std::string& text,
            float fontSize = 20.0f,
            Color32 color = CBLACK
        );

        ~Text() override = default;

        void draw() override;

        void update() override;

        void setText(const std::string& text);

        std::string getText() const;

        void setFontSize(float fontSize);

        float getFontSize() const;

        void setColor(Color32 color);

        Color32 getColor() const;

        void setSize(float width, float height) override;

        float getWidth() const;
        void setX(float x);
        void setY(float y);

    private:
        std::string _text;
        float _fontSize;
        Color32 _color;
        std::shared_ptr<IDisplay> _display;
};
