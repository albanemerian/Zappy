/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Raylib
*/

#include <iostream>
#include <string>
#include <memory>
#include "Raylib.hpp"
#include "../Utils/GamepadConstants.hpp"


Raylib::Raylib()
{
    this->_raylib = std::make_unique<RayLibEnc>();
}

Vector2i Raylib::getScreenSize()
{
    return {this->_raylib->getScreenWidth(), this->_raylib->getScreenHeight()};
}

Vector2i Raylib::getMonitorSize()
{
    return {this->_raylib->getMonitorWidth(0), this->_raylib->getMonitorHeight(0)};
}

void Raylib::initWindow(int width, int height, std::string title)
{
    this->_raylib->initWindow(width, height, title);
}

void Raylib::initCamera()
{
    return this->_raylib->initCamera();
}

bool Raylib::isWindowReady()
{
    return this->_raylib->isWindowReady();
}

void Raylib::setTargetFPS(unsigned int FPS)
{
    return this->_raylib->setTargetFPS(FPS);
}

bool Raylib::isOpen()
{
    return !this->_raylib->windowShouldClose();
}

void Raylib::closeWindow()
{
    return this->_raylib->closeWindow();
}

int Raylib::getKeyId(Key key)
{
    switch (key)
    {
    case TAB:
        return KEY_TAB;
    case ESC:
        return KEY_ESCAPE;
    case UP:
        return KEY_UP;
    case DOWN:
        return KEY_DOWN;
    case RIGHT:
        return KEY_RIGHT;
    case LEFT:
        return KEY_LEFT;
    case H:
        return KEY_H;
    case C:
        return KEY_C;
    case GM_PD_H:
        return GAMEPAD_BUTTON_SELECT;
    case GM_PD_LEFT_SHOULDER:
        return GAMEPAD_BUTTON_LEFT_SHOULDER;
    case GM_PD_RIGHT_SHOULDER:
        return GAMEPAD_BUTTON_RIGHT_SHOULDER;
    case GM_PD_LEFT_TRIGGER:
        return GAMEPAD_BUTTON_LEFT_TRIGGER;
    case GM_PD_RIGHT_TRIGGER:
        return GAMEPAD_BUTTON_RIGHT_TRIGGER;
    case GM_PD_UP:
        return GAMEPAD_BUTTON_UP;
    case GM_PD_DOWN:
        return GAMEPAD_BUTTON_DOWN;
    case GM_PD_AXIS_RIGHT_X:
        return GAMEPAD_AXIS_RIGHT_X;
    case GM_PD_AXIS_RIGHT_Y:
        return GAMEPAD_AXIS_RIGHT_Y;
    case MOUSE_RIGHT:
        return MOUSE_RIGHT_BUTTON;
    case MOUSE_LEFT:
        return MOUSE_LEFT_BUTTON;
    default : {
        std::cerr << "NO KEY FOUND" << std::endl;
        return -1;
    }
    }
}

bool Raylib::isKeyReleased(int key)
{
    return this->_raylib->isKeyReleased(key);
}

bool Raylib::isKeyPressed(int key)
{
    return this->_raylib->isKeyPressed(key);
}

bool Raylib::isKeyDown(int key)
{
    return this->_raylib->isKeyDown(key);
}

bool Raylib::isGamepadButtonReleased(int key)
{
    return this->_raylib->isGamepadButtonReleased(0, key);
}

bool Raylib::isGamepadButtonPressed(int key)
{
    return this->_raylib->isGamepadButtonPressed(0, key);
}

bool Raylib::isGamepadButtonDown(int key)
{
    return this->_raylib->isGamepadButtonDown(0, key);
}

bool Raylib::isGamepadAvailable()
{
    return this->_raylib->isGamepadAvailable(0);
}

bool Raylib::isMouseButtonDown(int key)
{
    return this->_raylib->isMouseButtonDown(key);
}

bool Raylib::isMouseButtonReleased(int key)
{
    return this->_raylib->isMouseButtonReleased(key);
}

bool Raylib::isMouseButtonPressed(int key)
{
    return this->_raylib->isMouseButtonPressed(key);
}

Vector2f Raylib::getMousePosition()
{
    Vector2 pos = this->_raylib->getMousePosition();
    return {pos.x, pos.y};
}

void Raylib::setMousePosition(Vector2f pos)
{
    return this->_raylib->setMousePosition(pos.x, pos.y);
}

float Raylib::getMouseWheelMove()
{
    return this->_raylib->getMouseWheelMove();
}

float Raylib::getGamepadAxisMovement(int key)
{
    return this->_raylib->getGamepadAxisMovement(0, key);
}

void Raylib::setCameraPosition(Vector3f pos)
{
    return this->_raylib->setCameraPosition({pos.x, pos.y, pos.z});
}

void Raylib::setCameraTarget(Vector3f pos)
{
    return this->_raylib->setCameraTarget({pos.x, pos.y, pos.z});
}

Vector2f Raylib::getMouseDelta()
{
    auto pos = this->_raylib->getMouseDelta();
    return {pos.x, pos.y};
}

float Raylib::vector3DDistanceFromCamera(Vector3f target)
{
    auto camera = this->_raylib->getCamera();
    return this->_raylib->vector3Distance(camera.position, {target.x, target.y, target.z});
}

Vector3f Raylib::vector3SubtractFromCamera(Vector3f target)
{
    auto camera = this->_raylib->getCamera();
    auto res = this->_raylib->vector3Subtract(camera.position, {target.x, target.y, target.z});
    return {res.x, res.y, res.z};
}

Vector3f Raylib::vector3Normalize(Vector3f vec)
{
    auto res = this->_raylib->vector3Normalize({vec.x, vec.y, vec.z});
    return {res.x, res.y, res.z};
}

void Raylib::enableCursor()
{
    return this->_raylib->enableCursor();
}

void Raylib::disableCursor()
{
    return this->_raylib->disableCursor();
}

float Raylib::getFrameTime()
{
    return this->_raylib->getFrameTime();
}

int Raylib::getFPS()
{
    return this->_raylib->getFPS();
}

void Raylib::updateCameraFreeMode(float camMovingSpeed, float camRotaSpeed)
{
    return this->_raylib->updateCameraFreeMode(camMovingSpeed, camRotaSpeed);
}

InputType Raylib::getLastInputType() const
{
    return this->_raylib->getLastInputType();
}

void Raylib::updateLastInputType()
{
    return this->_raylib->updateLastInputType();
}

float Raylib::measureText(const std::string& text, float fontSize) const
{
    return this->_raylib->measureText(text, fontSize);
}

bool Raylib::checkCollisionPointRec(Vector2f fpoint, FloatRect frect)
{
    Rectangle rect = {frect.x, frect.y, frect.width, frect.height};
    Vector2 point = {fpoint.x, fpoint.y};
    return this->_raylib->checkCollisionPointRec(point, rect);
}

Ray3D Raylib::getMouseRay(Vector2f mousePosition)
{
    Ray ray = this->_raylib->getMouseRay({mousePosition.x, mousePosition.y});
    return {{ray.position.x, ray.position.y, ray.position.z},
            {ray.direction.x, ray.direction.y, ray.direction.z}};
}

RayCollision3D Raylib::getRayCollisionBox(Ray3D ray, BoundingBox3D box)
{
    Ray raylibRay = {{ray.position.x, ray.position.y, ray.position.z},
                     {ray.direction.x, ray.direction.y, ray.direction.z}};
    BoundingBox raylibBox = {{box.min.x, box.min.y, box.min.z},
                             {box.max.x, box.max.y, box.max.z}};

    RayCollision collision = this->_raylib->getRayCollisionBox(raylibRay, raylibBox);
    return {collision.hit, collision.distance,
            {collision.point.x, collision.point.y, collision.point.z},
            {collision.normal.x, collision.normal.y, collision.normal.z}};
}

RayCollision3D Raylib::getRayCollisionSphere(Ray3D ray, Vector3f center, float radius)
{
    Ray raylibRay = {{ray.position.x, ray.position.y, ray.position.z},
                     {ray.direction.x, ray.direction.y, ray.direction.z}};
    Vector3 raylibCenter = {center.x, center.y, center.z};

    RayCollision collision = this->_raylib->getRayCollisionSphere(raylibRay,
        raylibCenter, radius);
    return {collision.hit, collision.distance,
            {collision.point.x, collision.point.y, collision.point.z},
            {collision.normal.x, collision.normal.y, collision.normal.z}};
}

bool Raylib::checkCollisionBoxes(BoundingBox3D box1, BoundingBox3D box2)
{
    BoundingBox raylibBox1 = {{box1.min.x, box1.min.y, box1.min.z},
                              {box1.max.x, box1.max.y, box1.max.z}};
    BoundingBox raylibBox2 = {{box2.min.x, box2.min.y, box2.min.z},
                              {box2.max.x, box2.max.y, box2.max.z}};

    return this->_raylib->checkCollisionBoxes(raylibBox1, raylibBox2);
}

Ray3D Raylib::getMouseRayFromCurrent()
{
    Vector2f mousePos = getMousePosition();
    return getMouseRay(mousePos);
}

BoundingBox3D Raylib::createBoundingBox(Vector3f center, Vector3f size)
{
    Vector3 centerRaylib = {center.x, center.y, center.z};
    Vector3 sizeRaylib = {size.x, size.y, size.z};

    BoundingBox box = this->_raylib->createBoundingBox(centerRaylib, sizeRaylib);
    return {{box.min.x, box.min.y, box.min.z},
            {box.max.x, box.max.y, box.max.z}};
}

BoundingBox3D Raylib::createBoundingBoxFromMinMax(Vector3f min, Vector3f max)
{
    Vector3 minRaylib = {min.x, min.y, min.z};
    Vector3 maxRaylib = {max.x, max.y, max.z};

    BoundingBox box = this->_raylib->createBoundingBoxFromMinMax(minRaylib, maxRaylib);
    return {{box.min.x, box.min.y, box.min.z},
            {box.max.x, box.max.y, box.max.z}};
}

void Raylib::endScissorMode()
{
    return this->_raylib->endScissorMode();
}

void Raylib::beginScissorMode(IntRect data)
{
    return this->_raylib->beginScissorMode(data.x, data.y, data.width, data.height);
}

void Raylib::drawCube(Vector3f pos, float width, float height, float length, Color32 color)
{
    return this->_raylib->drawCube({pos.x, pos.y, pos.z}, width, height, length,
    {color.r, color.g, color.b, color.a});
}

void Raylib::drawCubeWires(Vector3f pos, float width, float height, float length,
    Color32 color)
{
    return this->_raylib->drawCubeWires({pos.x, pos.y, pos.z}, width, height,
        length, {color.r, color.g, color.b, color.a});
}

void Raylib::drawSphere(Vector3f pos, float radius, Color32 color)
{
    return this->_raylib->drawSphere({pos.x, pos.y, pos.z}, radius,
        {color.r, color.g, color.b, color.a});
}

void Raylib::drawSphereWires(Vector3f pos, float radius, int rings, int slices, Color32 color)
{
    return this->_raylib->drawSphereWires({pos.x, pos.y, pos.z}, radius, rings,
        slices, {color.r, color.g, color.b, color.a});
}

void Raylib::drawCylinder(Vector3f pos, float radiusTop, float radiusBottom,
    float height, int slices, Color32 color)
{
    return this->_raylib->drawCylinder({pos.x, pos.y, pos.z}, radiusTop, radiusBottom,
        height, slices, {color.r, color.g, color.b, color.a});
}

void Raylib::drawCylinderWires(Vector3f pos, float radiusTop, float radiusBottom,
    float height, int slices, Color32 color)
{
    return this->_raylib->drawCylinderWires({pos.x, pos.y, pos.z}, radiusTop, radiusBottom,
        height, slices, {color.r, color.g, color.b, color.a});
}

void Raylib::drawCylinderEx(Vector3f startPos, Vector3f endPos, float startRadius,
    float endRadius, int sides, Color32 color)
{
    return this->_raylib->drawCylinderEx({startPos.x, startPos.y, startPos.z},
        {endPos.x, endPos.y, endPos.z}, startRadius, endRadius, sides,
        {color.r, color.g, color.b, color.a});
}

void Raylib::drawPlane(Vector3f pos, Vector2f size, Color32 color)
{
    return this->_raylib->drawPlane({pos.x, pos.y, pos.z}, {size.x, size.y},
        {color.r, color.g, color.b, color.a});
}

void Raylib::drawLine3D(Vector3f startPos, Vector3f endPos, Color32 color)
{
    return this->_raylib->drawLine3D({startPos.x, startPos.y, startPos.z},
        {endPos.x, endPos.y, endPos.z}, {color.r, color.g, color.b, color.a});
}

void Raylib::drawModelEx(const std::string &id, Vector3f pos, Vector3f rota, float angle,
    Vector3f scale, Color32 color)
{
    return this->_raylib->drawModelEx(id, {pos.x, pos.y, pos.z}, {rota.x, rota.y, rota.z},
        angle, {scale.x, scale.y, scale.z}, {color.r, color.g, color.b, color.a});
}

void Raylib::drawText(const std::string &text, float x, float y, float fontSize,
    Color32 color)
{
    return this->_raylib->drawText(text, x, y, fontSize,
        {color.r, color.g, color.b, color.a});
}

void Raylib::drawTextEx(const std::string &text, float x, float y, float fontSize,
    float spacing, Color32 color)
{
    return this->_raylib->drawTextEx(text, x, y, fontSize, spacing,
        {color.r, color.g, color.b, color.a});
}

void Raylib::drawRectangleRec(FloatRect frect, Color32 color)
{
    Rectangle rect = {frect.x, frect.y, frect.width, frect.height};
    return this->_raylib->drawRectangleRec(rect, {color.r, color.g, color.b, color.a});
}

void Raylib::drawCircle(float centerX, float centerY, float radius, Color32 color)
{
    return this->_raylib->drawCircle(centerX, centerY, radius,
        {color.r, color.g, color.b, color.a});
}

void Raylib::drawCircleLines(float centerX, float centerY, float radius, Color32 color)
{
    return this->_raylib->drawCircleLines(centerX, centerY, radius,
        {color.r, color.g, color.b, color.a});
}

void Raylib::end3DMode()
{
    return this->_raylib->end3DMode();
}

void Raylib::begin3DMode()
{
    return this->_raylib->begin3DMode();
}


void Raylib::beginDrawing()
{
    return this->_raylib->beginDrawing();
}

void Raylib::endDrawing()
{
    return this->_raylib->endDrawing();
}

void Raylib::clearBackground(Color32 color)
{
    return this->_raylib->clearBackground({color.r, color.g, color.b, color.a});
}

bool Raylib::loadModel(const std::string& id, const std::string& filepath, Vector3f center)
{
    return this->_raylib->loadModel(id, filepath, {center.x, center.y, center.z});
}

bool Raylib::loadTexture(const std::string& id, const std::string& filepath)
{
    if (this->_raylib->hasTexture(id)) {
        std::cout << "Texture " << id << " already exists" << std::endl;
        return true;
    }

    Texture2D texture = this->_raylib->loadTextureFromFile(filepath);
    if (texture.id == 0) {
        std::cout << "Failed to load texture " << filepath << std::endl;
        return false;
    }

    this->_raylib->addTexture(id, texture);
    return true;
}

void Raylib::drawTexture(const std::string& id, float x, float y, Color32 tint)
{
    if (!this->_raylib->hasTexture(id)) {
        std::cout << "Texture " << id << " not found" << std::endl;
        return;
    }

    Texture2D texture = this->_raylib->getTexture(id);
    this->_raylib->drawTextureEx(texture, {x, y}, {tint.r, tint.g, tint.b, tint.a});
}

void Raylib::drawTextureScaled(
    const std::string& id,
    float x,
    float y,
    float width,
    float height,
    Color32 tint)
{
    if (!this->_raylib->hasTexture(id)) {
        std::cout << "Texture " << id << " not found" << std::endl;
        return;
    }

    Texture2D texture = this->_raylib->getTexture(id);
    this->_raylib->drawTextureScaled(
        texture,
        x,
        y,
        width,
        height,
        {tint.r, tint.g, tint.b, tint.a});
}

Vector2f Raylib::getTextureSize(const std::string& id) const
{
    if (!this->_raylib->hasTexture(id)) {
        std::cout << "Texture " << id << " not found" << std::endl;
        return {0.0f, 0.0f};
    }

    Texture2D texture = this->_raylib->getTexture(id);
    return {static_cast<float>(texture.width), static_cast<float>(texture.height)};
}

bool Raylib::loadSkybox(const std::string& id, const std::string& filepath)
{
    return this->_raylib->loadSkybox(id, filepath);
}

void Raylib::drawSkybox(const std::string& id)
{
    this->_raylib->drawSkybox(id);
}

bool Raylib::loadFont(const std::string& id, const std::string& filepath)
{
    return this->_raylib->loadFont(id, filepath);
}

float Raylib::getTime() const
{
    return this->_raylib->getTime();
}
