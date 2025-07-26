/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Slider
*/

#include <chrono>
#include <algorithm>
#include <cmath>
#include <string>
#include <memory>
#include "Slider.hpp"

Slider::Slider(
    std::shared_ptr<IDisplay> display,
    float x, float y,
    float width, float height,
    float minValue, float maxValue,
    float initialValue,
    const std::string& text,
    std::function<void(float)> onValueChanged
) : AUIElement(display, x, y, width, height),
    _value(initialValue),
    _minValue(minValue),
    _maxValue(maxValue),
    _text(text),
    _onValueChanged(onValueChanged),
    _isDragging(false),
    _sliderTrackWidth(width * 0.6f),
    _sliderHandleRadius(height * 0.15f),
    _trackColor({200, 200, 200, 255}),
    _fillColor({100, 150, 255, 255}),
    _handleColor({255, 255, 255, 255}),
    _textColor({255, 255, 255, 255})
{
    _value = std::max(_minValue, std::min(_maxValue, _value));
}

void Slider::draw()
{
    if (!_visible)
        return;

    float textFontSize = _bounds.height * 0.3f;

    float textX = _bounds.x;
    float textY = _bounds.y;
    _display->drawText(_text, textX, textY, textFontSize, _textColor);

    std::string valueText = std::to_string(static_cast<int>(_value));
    float valueFontSize = _bounds.height * 0.25f;
    float textWidth = _display->measureText(_text, textFontSize);
    float valueX = textX + textWidth + 7.5f;
    float valueY = _bounds.y;
    _display->drawText(valueText, valueX, valueY + 2.0f, valueFontSize, _textColor);

    float sliderY = _bounds.y + textFontSize + 10.0f;
    float sliderStartX = _bounds.x;

    float trackThickness = _bounds.height * 0.15f;
    FloatRect trackRect = {
        sliderStartX,
        sliderY - trackThickness * 0.5f,
        _sliderTrackWidth,
        trackThickness
    };
    _display->drawRectangleRec(trackRect, _trackColor);

    float handlePos = getHandlePosition();
    FloatRect fillRect = {
        sliderStartX,
        sliderY - trackThickness * 0.5f,
        handlePos - sliderStartX,
        trackThickness
    };
    _display->drawRectangleRec(fillRect, _fillColor);

    _display->drawCircle(handlePos, sliderY, _sliderHandleRadius, _handleColor);
    _display->drawCircleLines(handlePos, sliderY, _sliderHandleRadius, {100, 100, 100, 255});
}

void Slider::update()
{
    if (!_visible)
        return;

    Vector2f mousePos = _display->getMousePosition();

    if (_display->isMouseButtonPressed(this->_display->getKeyId(MOUSE_LEFT))) {
        if (isMouseOverHandle(mousePos.x, mousePos.y)) {
            _isDragging = true;
        } else if (isMouseOverTrack(mousePos.x, mousePos.y)) {
            float oldValue = _value;
            updateValueFromMousePosition(mousePos.x);
            if (oldValue != _value)
                _onValueChanged(_value);
            _isDragging = true;
        }
    }

    if (_isDragging) {
        if (_display->isMouseButtonDown(this->_display->getKeyId(MOUSE_LEFT))) {
            float oldValue = _value;
            updateValueFromMousePosition(mousePos.x);
            if (oldValue != _value)
                _onValueChanged(_value);
        } else {
            _isDragging = false;
        }
    }
}

void Slider::setValue(float value)
{
    _value = std::max(_minValue, std::min(_maxValue, value));
}

float Slider::getValue() const
{
    return _value;
}

void Slider::setMinValue(float minValue)
{
    _minValue = minValue;
    if (_value < _minValue) {
        setValue(_minValue);
    }
}

void Slider::setMaxValue(float maxValue)
{
    _maxValue = maxValue;
    if (_value > _maxValue)
        setValue(_maxValue);
}

float Slider::getMinValue() const
{
    return _minValue;
}

float Slider::getMaxValue() const
{
    return _maxValue;
}

void Slider::setText(const std::string& text)
{
    _text = text;
}

std::string Slider::getText() const
{
    return _text;
}

void Slider::setSize(float width, float height)
{
    AUIElement::setSize(width, height);
    _sliderTrackWidth = width * 0.6f;
    _sliderHandleRadius = height * 0.15f;
}

void Slider::updateValueFromMousePosition(float mouseX)
{
    float sliderStartX = _bounds.x;
    float sliderEndX = sliderStartX + _sliderTrackWidth;

    float clampedX = std::max(sliderStartX, std::min(sliderEndX, mouseX));

    float ratio = (clampedX - sliderStartX) / _sliderTrackWidth;
    float newValue = _minValue + ratio * (_maxValue - _minValue);

    setValue(newValue);
}

float Slider::getHandlePosition() const
{
    float sliderStartX = _bounds.x;

    float ratio = (_value - _minValue) / (_maxValue - _minValue);
    return sliderStartX + ratio * _sliderTrackWidth;
}

bool Slider::isMouseOverHandle(float mouseX, float mouseY) const
{
    float handleX = getHandlePosition();
    float textFontSize = _bounds.height * 0.3f;
    float handleY = _bounds.y + textFontSize + 10.0f;

    float dx = mouseX - handleX;
    float dy = mouseY - handleY;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance <= _sliderHandleRadius;
}

bool Slider::isDragging() const
{
    return _isDragging;
}

bool Slider::isMouseOverTrack(float mouseX, float mouseY) const
{
    float textFontSize = _bounds.height * 0.3f;
    float sliderY = _bounds.y + textFontSize + 10.0f;
    float sliderStartX = _bounds.x;
    float trackThickness = _bounds.height * 0.15f;

    FloatRect trackRect = {
        sliderStartX,
        sliderY - trackThickness * 0.5f,
        _sliderTrackWidth,
        trackThickness
    };

    return _display->checkCollisionPointRec({mouseX, mouseY}, trackRect);
}
