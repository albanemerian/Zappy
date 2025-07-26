/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** ImageButton
*/

#include <string>
#include <memory>
#include "ImageButton.hpp"

ImageButton::ImageButton(
    std::shared_ptr<IDisplay> display,
    std::shared_ptr<IAudio> audio,
    float x, float y,
    float width, float height,
    const std::string& imagePath,
    std::function<void()> callback
) : Image(display, x, y, width, height, imagePath),
    _callback(callback),
    _audio(audio),
    _isHovered(false),
    _isPressed(false)
{
}

void ImageButton::update()
{
    this->Image::update();
    if (!this->_visible)
        return;
    Vector2f mousePoint = _display->getMousePosition();
    this->_isHovered = contains(mousePoint.x, mousePoint.y);
    bool wasPressed = this->_isPressed;
    this->_isPressed = this->_isHovered && this->_display->isMouseButtonDown(
        this->_display->getKeyId(MOUSE_LEFT));
    if (wasPressed && !this->_isPressed && this->_isHovered && this->_callback) {
        if (this->_audio) {
            this->_audio->playSound("click", this->_audio->getSFXVolumeLevel());
        }
        this->_callback();
    }
}

void ImageButton::setCallback(std::function<void()> callback)
{
    this->_callback = callback;
}

std::function<void()> ImageButton::getCallback() const
{
    return this->_callback;
}
