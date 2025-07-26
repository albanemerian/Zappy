/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Image
*/

#include "Image.hpp"
#include <memory>
#include <string>

Image::Image(
    std::shared_ptr<IDisplay> display,
    float x, float y,
    float width, float height,
    const std::string& imagePath
) : AUIElement(display, x, y, width, height),
    _imagePath(imagePath),
    _tint({255, 255, 255, 255}),
    _maintainAspectRatio(true),
    _imageLoaded(false)
{
    loadImage();
}

void Image::draw()
{
    if (!this->_visible)
        return;

    if (this->_imageLoaded) {
        this->_display->drawTextureScaled(this->_imagePath, this->_bounds.x, this->_bounds.y,
            this->_bounds.width, this->_bounds.height, this->_tint);
    } else {
        Color32 placeholderColor = {200, 200, 200, 128};
        this->_display->drawRectangleRec(this->_bounds, placeholderColor);

        float fontSize = this->_bounds.height * 0.2f;
        std::string placeholderText = "No Image";
        float textWidth = this->_display->measureText(placeholderText, fontSize);
        float textX = this->_bounds.x + (this->_bounds.width - textWidth) * 0.5f;
        float textY = this->_bounds.y + (this->_bounds.height - fontSize) * 0.5f;

        Color32 textColor = {100, 100, 100, 255};
        this->_display->drawText(placeholderText, textX, textY, fontSize, textColor);
    }
}

void Image::update()
{
    if (!this->_visible)
        return;
}

void Image::setImagePath(const std::string& imagePath)
{
    if (this->_imagePath != imagePath) {
        this->_imagePath = imagePath;
        loadImage();
    }
}

std::string Image::getImagePath() const
{
    return this->_imagePath;
}

void Image::setTint(Color32 tint)
{
    this->_tint = tint;
}

Color32 Image::getTint() const
{
    return this->_tint;
}

void Image::setSize(float width, float height)
{
    if (this->_maintainAspectRatio) {
        float aspectRatio = this->_bounds.width / this->_bounds.height;
        float newAspectRatio = width / height;
        if (newAspectRatio > aspectRatio)
            width = height * aspectRatio;
        else
            height = width / aspectRatio;
    }
    AUIElement::setSize(width, height);
}

void Image::setMaintainAspectRatio(bool maintain)
{
    this->_maintainAspectRatio = maintain;
}

bool Image::getMaintainAspectRatio() const
{
    return this->_maintainAspectRatio;
}

void Image::loadImage()
{
    if (!this->_imagePath.empty()) {
        this->_imageLoaded = this->_display->loadTexture(this->_imagePath, this->_imagePath);
    } else {
        this->_imageLoaded = false;
    }
}
