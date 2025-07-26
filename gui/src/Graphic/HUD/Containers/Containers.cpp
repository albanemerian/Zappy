/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Containers
*/

#include <string>
#include <memory>
#include "Containers.hpp"
#include "../Checkbox/Checkbox.hpp"

Containers::Containers(
    std::shared_ptr<IDisplay> display,
    std::shared_ptr<IAudio> audio,
    float x,
    float y,
    float width,
    float height,
    Color32 backgroundColor)
    : AContainers(display, x, y, width, height),
      _audio(audio),
      _elements()
{
    _backgroundColor = backgroundColor;
}

void Containers::draw()
{
    if (!_visible)
        return;

    if (_hasBackground) {
        this->_display->drawRectangleRec(_bounds, _backgroundColor);
    }

    this->_display->beginScissorMode(
        {
            static_cast<int>(_bounds.x),
            static_cast<int>(_bounds.y),
            static_cast<int>(_bounds.width),
            static_cast<int>(_bounds.height)
        }
    );

    for (auto& pair : _elements) {
        if (pair.first.find("scrollbar") != std::string::npos) {
            continue;
        }
        pair.second->draw();
    }

    this->_display->endScissorMode();

    for (auto& pair : _elements) {
        if (pair.first.find("scrollbar") != std::string::npos) {
            pair.second->draw();
        }
    }
}

void Containers::update()
{
    if (!_visible)
        return;

    for (auto& pair : _elements) {
        pair.second->update();
    }
}

void Containers::setBackgroundColor(Color32 color)
{
    _backgroundColor = color;
}

bool Containers::addElement(const std::string& id, std::shared_ptr<IUIElement> element)
{
    if (_elements.find(id) != _elements.end())
        return false;

    FloatRect elemBounds = element->getBounds();

    auto button = std::dynamic_pointer_cast<Button>(element);
    auto text = std::dynamic_pointer_cast<Text>(element);
    auto slider = std::dynamic_pointer_cast<Slider>(element);
    auto image = std::dynamic_pointer_cast<Image>(element);
    auto imageButton = std::dynamic_pointer_cast<ImageButton>(element);
    auto checkbox = std::dynamic_pointer_cast<Checkbox>(element);

    float xPercent = (elemBounds.x / _bounds.width) * 100.0f;
    float yPercent = (elemBounds.y / _bounds.height) * 100.0f;
    float widthPercent = (elemBounds.width / _bounds.width) * 100.0f;
    float heightPercent = (elemBounds.height / _bounds.height) * 100.0f;

    if (button) {
        button->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);
    } else if (text) {
        text->setRelativePosition(xPercent, yPercent, 0.0f, heightPercent);
    } else if (slider) {
        slider->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);
    } else if (image) {
        image->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);
    } else if (imageButton) {
        imageButton->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);
    } else if (checkbox) {
        checkbox->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);
    }

    element->setPosition(_bounds.x + elemBounds.x, _bounds.y + elemBounds.y);

    _elements[id] = element;
    return true;
}

std::shared_ptr<IUIElement> Containers::getElement(const std::string& id) const
{
    auto it = _elements.find(id);
    if (it != _elements.end())
        return it->second;

    return nullptr;
}

bool Containers::removeElement(const std::string& id)
{
    auto it = _elements.find(id);
    if (it != _elements.end()) {
        _elements.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Button> Containers::addButton(
    const std::string& id,
    float x, float y,
    float width, float height,
    const std::string& text,
    std::function<void()> callback
)
{
    auto button = std::make_shared<Button>(this->_display, _audio, x, y, width,
        height, text, callback);

    if (addElement(id, button))
        return button;

    return nullptr;
}

std::shared_ptr<Button> Containers::addButton(
    const std::string& id,
    float x, float y,
    float width, float height,
    const std::string& text,
    std::function<void()> callback,
    Color32 normalColor,
    Color32 hoverColor,
    Color32 pressedColor,
    Color32 textColor
)
{
    auto button = std::make_shared<Button>(this->_display, _audio, x, y, width, height,
        text, callback);
    button->setColors(normalColor, hoverColor, pressedColor, textColor);

    if (addElement(id, button))
        return button;

    return nullptr;
}

std::shared_ptr<Text> Containers::addText(
    const std::string& id,
    float x, float y,
    const std::string& text,
    float fontSize,
    Color32 color
)
{
    auto textElement = std::make_shared<Text>(this->_display, x, y, text, fontSize, color);

    if (addElement(id, textElement))
        return textElement;

    return nullptr;
}

std::shared_ptr<Image> Containers::addImage(
    const std::string& id,
    float x, float y,
    float width, float height,
    const std::string& imagePath
)
{
    auto image = std::make_shared<Image>(this->_display, x, y, width, height, imagePath);

    if (addElement(id, image))
        return image;

    return nullptr;
}

std::shared_ptr<Image> Containers::addImage(
    const std::string& id,
    float x, float y,
    float width, float height,
    const std::string& imagePath,
    Color32 tint
)
{
    auto image = std::make_shared<Image>(this->_display, x, y, width, height, imagePath);
    image->setTint(tint);

    if (addElement(id, image))
        return image;

    return nullptr;
}

void Containers::clearElements()
{
    _elements.clear();
}

void Containers::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    (void)oldWidth;
    (void)oldHeight;
    (void)newWidth;
    (void)newHeight;
    updatePositionFromRelative();

    for (auto& pair : _elements) {
        auto button = std::dynamic_pointer_cast<Button>(pair.second);
        auto text = std::dynamic_pointer_cast<Text>(pair.second);
        auto slider = std::dynamic_pointer_cast<Slider>(pair.second);
        auto image = std::dynamic_pointer_cast<Image>(pair.second);
        auto imageButton = std::dynamic_pointer_cast<ImageButton>(pair.second);

        UIRelativePosition elemRelPos;
        bool hasRelativePos = false;

        if (button) {
            elemRelPos = button->getRelativePosition();
            hasRelativePos = true;
        } else if (text) {
            elemRelPos = text->getRelativePosition();
            hasRelativePos = true;
        } else if (slider) {
            elemRelPos = slider->getRelativePosition();
            hasRelativePos = true;
        } else if (image) {
            elemRelPos = image->getRelativePosition();
            hasRelativePos = true;
        } else if (imageButton) {
            elemRelPos = imageButton->getRelativePosition();
            hasRelativePos = true;
        }

        if (hasRelativePos) {
            float elemNewX = _bounds.x + (_bounds.width * elemRelPos.xPercent / 100.0f);
            float elemNewY = _bounds.y + (_bounds.height * elemRelPos.yPercent / 100.0f);
            float elemNewWidth = _bounds.width * elemRelPos.widthPercent / 100.0f;
            float elemNewHeight = _bounds.height * elemRelPos.heightPercent / 100.0f;

            pair.second->setPosition(elemNewX, elemNewY);

            if (button) {
                button->setSize(elemNewWidth, elemNewHeight);
            } else if (text) {
                text->setFontSize(elemNewHeight);
            } else if (slider) {
                slider->setSize(elemNewWidth, elemNewHeight);
            } else if (image) {
                image->setSize(elemNewWidth, elemNewHeight);
            } else if (imageButton) {
                imageButton->setSize(elemNewWidth, elemNewHeight);
            }
        } else {
            FloatRect elemBounds = pair.second->getBounds();

            float elemRelXPercent = ((elemBounds.x - _bounds.x) / _bounds.width) * 100.0f;
            float elemRelYPercent = ((elemBounds.y - _bounds.y) / _bounds.height) * 100.0f;
            float elemWidthPercent = (elemBounds.width / _bounds.width) * 100.0f;
            float elemHeightPercent = (elemBounds.height / _bounds.height) * 100.0f;

            float elemNewX = _bounds.x + (_bounds.width * elemRelXPercent / 100.0f);
            float elemNewY = _bounds.y + (_bounds.height * elemRelYPercent / 100.0f);
            float elemNewWidth = _bounds.width * elemWidthPercent / 100.0f;
            float elemNewHeight = _bounds.height * elemHeightPercent / 100.0f;

            pair.second->setPosition(elemNewX, elemNewY);

            if (button) {
                button->setSize(elemNewWidth, elemNewHeight);
            } else if (text) {
                text->setFontSize(elemNewHeight);
            } else if (slider) {
                slider->setSize(elemNewWidth, elemNewHeight);
            }
        }
    }
}

std::shared_ptr<Button> Containers::addButtonPercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    const std::string& text,
    std::function<void()> callback
)
{
    float x = (_bounds.width * xPercent) / 100.0f;
    float y = (_bounds.height * yPercent) / 100.0f;
    float width = (_bounds.width * widthPercent) / 100.0f;
    float height = (_bounds.height * heightPercent) / 100.0f;

    auto button = std::make_shared<Button>(this->_display, _audio, x, y, width, height,
        text, callback);

    button->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);

    if (addElement(id, button))
        return button;

    return nullptr;
}

std::shared_ptr<Button> Containers::addButtonPercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    const std::string& text,
    std::function<void()> callback,
    Color32 normalColor,
    Color32 hoverColor,
    Color32 pressedColor,
    Color32 textColor
)
{
    auto button = addButtonPercent(
        id,
        xPercent,
        yPercent,
        widthPercent,
        heightPercent,
        text,
        callback);

    if (button)
        button->setColors(normalColor, hoverColor, pressedColor, textColor);

    return button;
}

std::shared_ptr<Text> Containers::addTextPercent(
    const std::string& id,
    float xPercent, float yPercent,
    const std::string& text,
    float fontSizePercent,
    Color32 color
)
{
    float x = (_bounds.width * xPercent) / 100.0f;
    float y = (_bounds.height * yPercent) / 100.0f;
    float fontSize = (_bounds.height * fontSizePercent) / 100.0f;

    auto textElement = std::make_shared<Text>(this->_display, x, y, text, fontSize, color);

    textElement->setRelativePosition(xPercent, yPercent, 0.0f, fontSizePercent);

    if (addElement(id, textElement))
        return textElement;

    return nullptr;
}

std::shared_ptr<Slider> Containers::addSlider(
    const std::string& id,
    float x, float y,
    float width, float height,
    float minValue, float maxValue,
    float initialValue,
    const std::string& text,
    std::function<void(float)> onValueChanged
)
{
    auto slider = std::make_shared<Slider>(_display, x, y, width, height,
        minValue, maxValue, initialValue, text, onValueChanged);

    if (addElement(id, slider))
        return slider;

    return nullptr;
}

std::shared_ptr<Slider> Containers::addSliderPercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    float minValue, float maxValue,
    float initialValue,
    const std::string& text,
    std::function<void(float)> onValueChanged
)
{
    float x = (_bounds.width * xPercent) / 100.0f;
    float y = (_bounds.height * yPercent) / 100.0f;
    float width = (_bounds.width * widthPercent) / 100.0f;
    float height = (_bounds.height * heightPercent) / 100.0f;

    auto slider = std::make_shared<Slider>(_display, x, y, width, height,
        minValue, maxValue, initialValue, text, onValueChanged);

    slider->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);

    if (addElement(id, slider))
        return slider;

    return nullptr;
}

float Containers::getWidth() const
{
    return AContainers::getWidth();
}

float Containers::getHeight() const
{
    return AContainers::getHeight();
}

std::shared_ptr<Image> Containers::addImagePercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    const std::string& imagePath
)
{
    float x = (_bounds.width * xPercent) / 100.0f;
    float y = (_bounds.height * yPercent) / 100.0f;
    float width = (_bounds.width * widthPercent) / 100.0f;
    float height = (_bounds.height * heightPercent) / 100.0f;

    auto image = std::make_shared<Image>(this->_display, x, y, width, height, imagePath);

    image->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);

    if (addElement(id, image))
        return image;

    return nullptr;
}

std::shared_ptr<Image> Containers::addImagePercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    const std::string& imagePath,
    Color32 tint
)
{
    auto image = addImagePercent(id, xPercent, yPercent, widthPercent, heightPercent,
        imagePath);

    if (image)
        image->setTint(tint);

    return image;
}

std::shared_ptr<ImageButton> Containers::addImageButton(
    const std::string& id,
    float x, float y,
    float width, float height,
    const std::string& imagePath,
    std::function<void()> callback
)
{
    auto imageButton = std::make_shared<ImageButton>(this->_display, _audio, x, y, width,
        height, imagePath, callback);

    if (addElement(id, imageButton))
        return imageButton;

    return nullptr;
}

std::shared_ptr<ImageButton> Containers::addImageButton(
    const std::string& id,
    float x, float y,
    float width, float height,
    const std::string& imagePath,
    std::function<void()> callback,
    Color32 tint
)
{
    auto imageButton = std::make_shared<ImageButton>(this->_display, _audio, x, y, width,
        height, imagePath, callback);
    imageButton->setTint(tint);

    if (addElement(id, imageButton))
        return imageButton;

    return nullptr;
}

std::shared_ptr<ImageButton> Containers::addImageButtonPercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthImage, float heightImage,
    const std::string& imagePath,
    std::function<void()> callback
)
{
    float x = (_bounds.width * xPercent) / 100.0f;
    float y = (_bounds.height * yPercent) / 100.0f;
    float width = widthImage;
    float height = heightImage;

    auto imageButton = std::make_shared<ImageButton>(this->_display, _audio, x, y, width,
        height, imagePath, callback);

    imageButton->setRelativePosition(xPercent, yPercent, widthImage, heightImage);

    if (addElement(id, imageButton))
        return imageButton;

    return nullptr;
}

std::shared_ptr<ImageButton> Containers::addImageButtonPercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    const std::string& imagePath,
    std::function<void()> callback,
    Color32 tint
)
{
    auto imageButton = addImageButtonPercent(id, xPercent, yPercent, widthPercent,
        heightPercent, imagePath, callback);

    if (imageButton)
        imageButton->setTint(tint);

    return imageButton;
}

std::shared_ptr<Checkbox> Containers::addCheckbox(
    const std::string& id,
    float x, float y,
    float width, float height,
    bool initialValue,
    std::function<void(bool)> callback
)
{
    auto checkbox = std::make_shared<Checkbox>(_display, _audio, x, y, width, height,
        initialValue, callback);

    if (addElement(id, checkbox))
        return checkbox;

    return nullptr;
}

std::shared_ptr<Checkbox> Containers::addCheckboxPercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    bool initialValue,
    std::function<void(bool)> callback
)
{
    float x = (_bounds.width * xPercent) / 100.0f;
    float y = (_bounds.height * yPercent) / 100.0f;
    float width = (_bounds.width * widthPercent) / 100.0f;
    float height = (_bounds.height * heightPercent) / 100.0f;

    auto checkbox = std::make_shared<Checkbox>(_display, _audio, x, y, width, height,
        initialValue, callback);

    checkbox->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);

    if (addElement(id, checkbox))
        return checkbox;

    return nullptr;
}
