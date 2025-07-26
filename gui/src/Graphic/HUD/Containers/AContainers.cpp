/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AContainers
*/

#include "AContainers.hpp"
#include <memory>

AContainers::AContainers(
    std::shared_ptr<IDisplay> display,
    float x,
    float y,
    float width,
    float height)
    : _display(display),
      _bounds({x, y, width, height}),
      _backgroundColor(CRAYWHITE),
      _visible(true),
      _hasBackground(true)
{
    Vector2i screenSize = this->_display->getScreenSize();
    int screenWidth = screenSize.x;
    int screenHeight = screenSize.y;

    _relativePos.xPercent = (x / screenWidth) * 100.0f;
    _relativePos.yPercent = (y / screenHeight) * 100.0f;
    _relativePos.widthPercent = (width / screenWidth) * 100.0f;
    _relativePos.heightPercent = (height / screenHeight) * 100.0f;
}

void AContainers::setPosition(float x, float y)
{
    _bounds.x = x;
    _bounds.y = y;

    Vector2i screenSize = this->_display->getScreenSize();
    int screenWidth = screenSize.x;
    int screenHeight = screenSize.y;

    _relativePos.xPercent = (x / screenWidth) * 100.0f;
    _relativePos.yPercent = (y / screenHeight) * 100.0f;
}

void AContainers::setSize(float width, float height)
{
    _bounds.width = width;
    _bounds.height = height;

    Vector2i screenSize = this->_display->getScreenSize();
    int screenWidth = screenSize.x;
    int screenHeight = screenSize.y;

    _relativePos.widthPercent = (width / screenWidth) * 100.0f;
    _relativePos.heightPercent = (height / screenHeight) * 100.0f;
}

FloatRect AContainers::getBounds() const
{
    return _bounds;
}

bool AContainers::contains(float x, float y) const
{
    return this->_display->checkCollisionPointRec({x, y}, _bounds);
}

void AContainers::setVisible(bool visible)
{
    _visible = visible;
}

bool AContainers::isVisible() const
{
    return _visible;
}

void AContainers::setRelativePosition(
    float xPercent,
    float yPercent,
    float widthPercent,
    float heightPercent)
{
    _relativePos.xPercent = xPercent;
    _relativePos.yPercent = yPercent;
    _relativePos.widthPercent = widthPercent;
    _relativePos.heightPercent = heightPercent;

    updatePositionFromRelative();
}

RelativePosition AContainers::getRelativePosition() const
{
    return _relativePos;
}

void AContainers::updatePositionFromRelative()
{
    Vector2i screenSize = this->_display->getScreenSize();
    int screenWidth = screenSize.x;
    int screenHeight = screenSize.y;

    _bounds.x = (screenWidth * _relativePos.xPercent) / 100.0f;
    _bounds.y = (screenHeight * _relativePos.yPercent) / 100.0f;
    _bounds.width = (screenWidth * _relativePos.widthPercent) / 100.0f;
    _bounds.height = (screenHeight * _relativePos.heightPercent) / 100.0f;
}

float AContainers::getWidth() const
{
    return _bounds.width;
}

float AContainers::getHeight() const
{
    return _bounds.height;
}
