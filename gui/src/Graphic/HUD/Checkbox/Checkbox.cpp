/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Checkbox
*/

#include <memory>
#include <string>
#include <algorithm>
#include "Checkbox.hpp"

Checkbox::Checkbox(
    std::shared_ptr<IDisplay> display,
    std::shared_ptr<IAudio> audio,
    float x, float y,
    float width, float height,
    bool initialValue,
    std::function<void(bool)> callback
) : AUIElement(display, x, y, width, height),
    _value(initialValue),
    _callback(callback),
    _normalColor({200, 50, 50, 255}),
    _hoverColor({220, 70, 70, 255}),
    _pressedColor({160, 40, 40, 255}),
    _checkColor({50, 200, 50, 255}),
    _isHovered(false),
    _isPressed(false),
    _display(display),
    _audio(audio)
{
    _checkboxSize = std::min(width, height);
}

void Checkbox::draw()
{
    if (!_visible)
        return;

    float checkboxX = _bounds.x;
    float checkboxY = _bounds.y + (_bounds.height - _checkboxSize) / 2.0f;

    FloatRect checkboxRect = {
        checkboxX,
        checkboxY,
        _checkboxSize,
        _checkboxSize
    };

    Color32 boxColor;
    if (_value) {
        boxColor = _checkColor;
        if (_isPressed) {
            boxColor = {40, 160, 40, 255};
        } else if (_isHovered) {
            boxColor = {70, 220, 70, 255};
        }
    } else {
        boxColor = _normalColor;
        if (_isPressed) {
            boxColor = _pressedColor;
        } else if (_isHovered) {
            boxColor = _hoverColor;
        }
    }

    float borderThickness = 2.0f;
    FloatRect borderRect = {
        checkboxX - borderThickness,
        checkboxY - borderThickness,
        _checkboxSize + 2 * borderThickness,
        _checkboxSize + 2 * borderThickness
    };
    _display->drawRectangleRec(borderRect, {0, 0, 0, 255});
    _display->drawRectangleRec(checkboxRect, boxColor);
}

void Checkbox::update()
{
    if (!_visible)
        return;

    Vector2f mousePoint = this->_display->getMousePosition();
    _isHovered = containsCheckbox(mousePoint.x, mousePoint.y);

    bool wasPressed = _isPressed;
    _isPressed = _isHovered && this->_display->isMouseButtonDown(
        this->_display->getKeyId(MOUSE_LEFT));

    if (wasPressed && !_isPressed && _isHovered) {
        _value = !_value;

        if (_audio)
            _audio->playSound("click", this->_audio->getSFXVolumeLevel());

        if (_callback)
            _callback(_value);
    }
}

void Checkbox::setCallback(std::function<void(bool)> callback)
{
    _callback = callback;
}

void Checkbox::setValue(bool value)
{
    _value = value;
}

bool Checkbox::getValue() const
{
    return _value;
}

void Checkbox::setColors(
    Color32 normalColor,
    Color32 hoverColor,
    Color32 pressedColor,
    Color32 checkColor
)
{
    _normalColor = normalColor;
    _hoverColor = hoverColor;
    _pressedColor = pressedColor;
    _checkColor = checkColor;
}

void Checkbox::setSize(float width, float height)
{
    AUIElement::setSize(width, height);
    _checkboxSize = std::min(width, height);
}

bool Checkbox::containsCheckbox(float x, float y) const
{
    float checkboxX = _bounds.x;
    float checkboxY = _bounds.y + (_bounds.height - _checkboxSize) / 2.0f;

    FloatRect checkboxRect = {
        checkboxX,
        checkboxY,
        _checkboxSize,
        _checkboxSize
    };

    return this->_display->checkCollisionPointRec({x, y}, checkboxRect);
}
