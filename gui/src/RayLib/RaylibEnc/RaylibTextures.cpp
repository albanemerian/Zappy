/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** TextureMethods
*/

#include <iostream>
#include <string>
#include "RayLibEnc.hpp"

Texture2D RayLibEnc::loadTextureFromFile(const std::string& filepath)
{
    return LoadTexture(filepath.c_str());
}

void RayLibEnc::drawTextureEx(Texture2D texture, Vector2 position, Color tint)
{
    DrawTexture(texture, position.x, position.y, tint);
}

void RayLibEnc::drawTextureScaled(Texture2D texture, float x, float y, float width,
    float height, Color tint)
{
    Rectangle source = {
        0.0f,
        0.0f,
        static_cast<float>(texture.width),
        static_cast<float>(texture.height)};
    Rectangle dest = { x, y, width, height };
    DrawTexturePro(texture, source, dest, {0, 0}, 0.0f, tint);
}

bool RayLibEnc::hasTexture(const std::string& id) const
{
    return _textures.find(id) != _textures.end();
}

Texture2D RayLibEnc::getTexture(const std::string& id) const
{
    auto it = _textures.find(id);

    if (it == _textures.end()) {
        Texture2D emptyTexture = {
            0, 0, 0, 0, 0
        };
        return emptyTexture;
    }
    return it->second;
}

void RayLibEnc::addTexture(const std::string& id, Texture2D texture)
{
    _textures[id] = texture;
}
