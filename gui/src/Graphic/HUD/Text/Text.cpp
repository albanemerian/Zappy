/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Text
*/

#include "Text.hpp"
#include <memory>
#include <string>

Text::Text(
    std::shared_ptr<IDisplay> display,
    float x, float y,
    const std::string& text,
    float fontSize,
    Color32 color
) : AUIElement(display, x, y, display->measureText(text, fontSize), fontSize),
    _text(text),
    _fontSize(fontSize),
    _color(color),
    _display(display)
{
    _bounds.width = this->_display->measureText(_text, _fontSize);
    _bounds.height = fontSize;
}

void Text::draw()
{
    if (!_visible)
        return;

    this->_display->drawText(_text, _bounds.x, _bounds.y, _fontSize, _color);
}

void Text::update()
{
}

void Text::setText(const std::string& text)
{
    _text = text;

    _bounds.width = this->_display->measureText(_text, _fontSize);
}

std::string Text::getText() const
{
    return _text;
}

void Text::setFontSize(float fontSize)
{
    _fontSize = fontSize;

    _bounds.width = this->_display->measureText(_text, _fontSize);
    _bounds.height = _fontSize;
}

float Text::getFontSize() const
{
    return _fontSize;
}

void Text::setColor(Color32 color)
{
    _color = color;
}

Color32 Text::getColor() const
{
    return _color;
}

void Text::setSize(float width, float height)
{
    (void)width;
    setFontSize(height);

    _bounds.width = this->_display->measureText(_text, _fontSize);
}

float Text::getWidth() const
{
    return _bounds.width;
}

void Text::setX(float x)
{
    _bounds.x = x;
}

void Text::setY(float y)
{
    _bounds.y = y;
}
