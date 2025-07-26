/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Slider
*/

#ifndef SLIDER_HPP_
#define SLIDER_HPP_

#include <string>
#include <functional>
#include <memory>

#include "../../../IDisplay.hpp"
#include "../UIElement/AUIElement.hpp"

class Slider : public AUIElement {
    public:
        Slider(
            std::shared_ptr<IDisplay> raylib,
            float x, float y,
            float width, float height,
            float minValue, float maxValue,
            float initialValue,
            const std::string& text,
            std::function<void(float)> onValueChanged
        );

        ~Slider() override = default;

        void draw() override;
        void update() override;
        bool isDragging() const;

        void setValue(float value);
        float getValue() const;
        void setMinValue(float minValue);
        void setMaxValue(float maxValue);
        float getMinValue() const;
        float getMaxValue() const;
        void setText(const std::string& text);
        std::string getText() const;

        void setSize(float width, float height) override;

    private:
        float _value;
        float _minValue;
        float _maxValue;
        std::string _text;
        std::function<void(float)> _onValueChanged;

        bool _isDragging;
        float _sliderTrackWidth;
        float _sliderHandleRadius;

        Color32 _trackColor;
        Color32 _fillColor;
        Color32 _handleColor;
        Color32 _textColor;

        void updateValueFromMousePosition(float mouseX);
        float getHandlePosition() const;
        bool isMouseOverHandle(float mouseX, float mouseY) const;
        bool isMouseOverTrack(float mouseX, float mouseY) const;
};

#endif /* !SLIDER_HPP_ */
