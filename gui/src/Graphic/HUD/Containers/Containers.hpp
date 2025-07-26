/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Containers
*/

#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>
#include <string>

#include "AContainers.hpp"
#include "../UIElement/IUIElement.hpp"
#include "../Button/Button.hpp"
#include "../Text/Text.hpp"
#include "../Slider/Slider.hpp"
#include "../Image/Image.hpp"
#include "../ImageButton/ImageButton.hpp"
#include "../Checkbox/Checkbox.hpp"
#include "../../../Audio/IAudio.hpp"
#include "../../../IDisplay.hpp"

class Containers : public AContainers {
    public:
        Containers(std::shared_ptr<IDisplay> display, std::shared_ptr<IAudio> audio,
            float x, float y, float width, float height,
            Color32 backgroundColor = {40, 40, 40, 200});

        ~Containers() override = default;

        void draw() override;

        void update() override;

        void setBackgroundColor(Color32 color);

        bool addElement(const std::string& id, std::shared_ptr<IUIElement> element);

        std::shared_ptr<IUIElement> getElement(const std::string& id) const;

        bool removeElement(const std::string& id);

        std::shared_ptr<Button> addButton(
            const std::string& id,
            float x, float y,
            float width, float height,
            const std::string& text,
            std::function<void()> callback
        );

        std::shared_ptr<Button> addButton(
            const std::string& id,
            float x, float y,
            float width, float height,
            const std::string& text,
            std::function<void()> callback,
            Color32 normalColor,
            Color32 hoverColor,
            Color32 pressedColor,
            Color32 textColor
        );

        std::shared_ptr<Text> addText(
            const std::string& id,
            float x, float y,
            const std::string& text,
            float fontSize = 20.0f,
            Color32 color = CBLACK
        );

        std::shared_ptr<Slider> addSlider(
            const std::string& id,
            float x, float y,
            float width, float height,
            float minValue, float maxValue,
            float initialValue,
            const std::string& text,
            std::function<void(float)> onValueChanged
        );

        std::shared_ptr<Slider> addSliderPercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            float minValue, float maxValue,
            float initialValue,
            const std::string& text,
            std::function<void(float)> onValueChanged
        );

        void clearElements();

        void handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight);

        std::shared_ptr<Button> addButtonPercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            const std::string& text,
            std::function<void()> callback
        );

        std::shared_ptr<Button> addButtonPercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            const std::string& text,
            std::function<void()> callback,
            Color32 normalColor,
            Color32 hoverColor,
            Color32 pressedColor,
            Color32 textColor
        );

        std::shared_ptr<Text> addTextPercent(
            const std::string& id,
            float xPercent, float yPercent,
            const std::string& text,
            float fontSizePercent = 5.0f,
            Color32 color = CBLACK
        );
        std::shared_ptr<Image> addImage(
            const std::string& id,
            float x, float y,
            float width, float height,
            const std::string& imagePath
        );

        std::shared_ptr<Image> addImage(
            const std::string& id,
            float x, float y,
            float width, float height,
            const std::string& imagePath,
            Color32 tint
        );

        std::shared_ptr<Image> addImagePercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            const std::string& imagePath
        );

        std::shared_ptr<Image> addImagePercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            const std::string& imagePath,
            Color32 tint
        );

        std::shared_ptr<ImageButton> addImageButton(
            const std::string& id,
            float x, float y,
            float width, float height,
            const std::string& imagePath,
            std::function<void()> callback
        );

        std::shared_ptr<ImageButton> addImageButton(
            const std::string& id,
            float x, float y,
            float width, float height,
            const std::string& imagePath,
            std::function<void()> callback,
            Color32 tint
        );

        std::shared_ptr<ImageButton> addImageButtonPercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            const std::string& imagePath,
            std::function<void()> callback
        );

        std::shared_ptr<ImageButton> addImageButtonPercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            const std::string& imagePath,
            std::function<void()> callback,
            Color32 tint
        );

        std::shared_ptr<Checkbox> addCheckbox(
            const std::string& id,
            float x, float y,
            float width, float height,
            bool initialValue,
            std::function<void(bool)> callback
        );

        std::shared_ptr<Checkbox> addCheckboxPercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            bool initialValue,
            std::function<void(bool)> callback
        );

                float getWidth() const;
                float getHeight() const;

    private:
        std::shared_ptr<IAudio> _audio;
        std::unordered_map<std::string, std::shared_ptr<IUIElement>> _elements;
};
