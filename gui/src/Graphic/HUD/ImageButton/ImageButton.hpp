/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** ImageButton
*/

#pragma once

#include <functional>
#include <string>
#include <memory>
#include "../Image/Image.hpp"
#include "../../../Audio/IAudio.hpp"

class ImageButton : public Image {
    public:
        ImageButton(
            std::shared_ptr<IDisplay> display,
            std::shared_ptr<IAudio> audio,
            float x, float y,
            float width, float height,
            const std::string& imagePath,
            std::function<void()> callback
        );

        ~ImageButton() override = default;

        void update() override;

        void setCallback(std::function<void()> callback);

        std::function<void()> getCallback() const;

    private:
        std::function<void()> _callback;
        std::shared_ptr<IAudio> _audio;
        bool _isHovered;
        bool _isPressed;
};
