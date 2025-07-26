/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AUIElement
*/

#include "AUIElement.hpp"
#include <memory>

AUIElement::AUIElement(
    std::shared_ptr<IDisplay> display,
    float x,
    float y,
    float width,
    float height)
    : _display(display), _bounds({x, y, width, height}), _visible(true)
{
    _relativePos.xPercent = 0;
    _relativePos.yPercent = 0;
    _relativePos.widthPercent = 0;
    _relativePos.heightPercent = 0;
}

void AUIElement::setPosition(float x, float y)
{
    _bounds.x = x;
    _bounds.y = y;
}

FloatRect AUIElement::getBounds() const
{
    return _bounds;
}

bool AUIElement::contains(float x, float y) const
{
    return this->_display->checkCollisionPointRec({x, y}, _bounds);
}

void AUIElement::setVisible(bool visible)
{
    _visible = visible;
}

bool AUIElement::isVisible() const
{
    return _visible;
}

void AUIElement::setSize(float width, float height)
{
    _bounds.width = width;
    _bounds.height = height;
}

void AUIElement::setRelativePosition(
    float xPercent,
    float yPercent,
    float widthPercent,
    float heightPercent)
{
    _relativePos.xPercent = xPercent;
    _relativePos.yPercent = yPercent;
    _relativePos.widthPercent = widthPercent;
    _relativePos.heightPercent = heightPercent;
}

UIRelativePosition AUIElement::getRelativePosition() const
{
    return _relativePos;
}
