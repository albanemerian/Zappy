/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Image
*/

#pragma once

#include <string>
#include <memory>

#include "../UIElement/AUIElement.hpp"
#include "../../../IDisplay.hpp"

class Image : public AUIElement {
    public:
        Image(
            std::shared_ptr<IDisplay> display,
            float x, float y,
            float width, float height,
            const std::string& imagePath
        );

        ~Image() override = default;

        void draw() override;

        void update() override;

        void setImagePath(const std::string& imagePath);

        std::string getImagePath() const;

        void setTint(Color32 tint);

        Color32 getTint() const;

        void setSize(float width, float height) override;

        void setMaintainAspectRatio(bool maintain);

        bool getMaintainAspectRatio() const;

    private:
        std::string _imagePath;
        Color32 _tint;
        bool _maintainAspectRatio;
        bool _imageLoaded;

        void loadImage();
};
