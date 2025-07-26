/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Raylib
*/

#ifndef RAYLIB_HPP_
#define RAYLIB_HPP_
#include <memory>
#include <string>
#include "../IDisplay.hpp"
#include "RaylibEnc/RayLibEnc.hpp"

class Raylib : public IDisplay {
    private:
        std::unique_ptr<RayLibEnc> _raylib;

    public:
        virtual Vector2i getMonitorSize();
        virtual Vector2i getScreenSize();

        virtual void initWindow(int width, int height, std::string);
        virtual void initCamera();

        virtual bool isWindowReady();
        virtual void setTargetFPS(unsigned int FPS);

        virtual bool isOpen();
        virtual void closeWindow();

        virtual int getKeyId(enum Key);

        virtual bool isKeyReleased(int key);
        virtual bool isKeyPressed(int key);
        virtual bool isKeyDown(int key);

        virtual bool isGamepadAvailable();

        virtual bool isGamepadButtonReleased(int key);
        virtual bool isGamepadButtonPressed(int key);
        virtual bool isGamepadButtonDown(int key);


        virtual bool isMouseButtonDown(int key);
        virtual bool isMouseButtonReleased(int key);
        virtual bool isMouseButtonPressed(int key);

        virtual Vector2f getMousePosition();
        virtual void setMousePosition(Vector2f);

        virtual float getMouseWheelMove();

        virtual float getGamepadAxisMovement(int key);

        virtual void setCameraPosition(Vector3f);

        virtual void setCameraTarget(Vector3f);

        virtual Vector2f getMouseDelta();

        virtual float vector3DDistanceFromCamera(Vector3f target);
        virtual Vector3f vector3SubtractFromCamera(Vector3f target);

        virtual Vector3f vector3Normalize(Vector3f);

        virtual void enableCursor();
        virtual void disableCursor();

        virtual float getFrameTime();
        virtual int getFPS();

        virtual void updateCameraFreeMode(float camMovingSpeed, float camRotaSpeed);

        virtual InputType getLastInputType() const;
        virtual void updateLastInputType();

        virtual float measureText(const std::string& text, float fontSize) const;

        virtual bool checkCollisionPointRec(Vector2f point, FloatRect rec);

        virtual Ray3D getMouseRay(Vector2f mousePosition);
        virtual RayCollision3D getRayCollisionBox(Ray3D ray, BoundingBox3D box);
        virtual RayCollision3D getRayCollisionSphere(Ray3D ray, Vector3f center, float radius);
        virtual bool checkCollisionBoxes(BoundingBox3D box1, BoundingBox3D box2);

        virtual Ray3D getMouseRayFromCurrent();
        virtual BoundingBox3D createBoundingBox(Vector3f center, Vector3f size);
        virtual BoundingBox3D createBoundingBoxFromMinMax(Vector3f min, Vector3f max);

        virtual void beginScissorMode(IntRect);
        virtual void endScissorMode();

        virtual void beginDrawing();
        virtual void endDrawing();

        virtual void clearBackground(Color32);

        virtual void begin3DMode();
        virtual void end3DMode();

        virtual bool loadModel(const std::string& id, const std::string& filepath,
            Vector3f center = {0.0f, 0.0f, 0.0f});

        virtual void drawCube(Vector3f position, float width, float height, float length,
            Color32 color);
        virtual void drawCubeWires(Vector3f position, float width, float height, float length,
            Color32 color);

        virtual void drawSphere(Vector3f position, float radius, Color32 color);
        virtual void drawSphereWires(Vector3f position, float radius, int rings, int slices,
            Color32 color);

        virtual void drawCylinder(Vector3f position, float radiusTop, float radiusBottom,
            float height, int slices, Color32 color);
        virtual void drawCylinderWires(Vector3f position, float radiusTop, float radiusBottom,
            float height, int slices, Color32 color);
        virtual void drawCylinderEx(Vector3f startPos, Vector3f endPos, float startRadius,
            float endRadius, int sides, Color32 color);

        virtual void drawPlane(Vector3f position, Vector2f size, Color32 color);

        virtual void drawLine3D(Vector3f startPos, Vector3f endPos, Color32 color);

        virtual void drawModelEx(const std::string& id, Vector3f position,
            Vector3f rotationAxis, float rotationAngle, Vector3f scale,
            Color32 tint = CWHITE);

        virtual void drawText(const std::string& text, float x, float y, float fontSize,
            Color32 color);

        virtual void drawTextEx(const std::string& text, float x, float y, float fontSize,
            float spacing, Color32 color);

        virtual void drawCircle(float centerX, float centerY, float radius,
            Color32 color);
        virtual void drawCircleLines(float centerX, float centerY,
            float radius, Color32 color);

        virtual void drawRectangleRec(FloatRect rec, Color32 color);

        virtual bool loadTexture(const std::string& id, const std::string& filepath);

        virtual bool loadFont(const std::string& id, const std::string& filepath);

        virtual void drawTexture(const std::string& id, float x, float y,
            Color32 tint = CWHITE);

        virtual void drawTextureScaled(const std::string& id, float x, float y, float width,
            float height, Color32 tint = CWHITE);

        virtual Vector2f getTextureSize(const std::string& id) const;

        virtual bool loadSkybox(const std::string& id, const std::string& filepath);

        virtual void drawSkybox(const std::string& id);

        virtual float getTime() const;

        Raylib();
        ~Raylib() = default;
};

#endif /* !RAYLIB_HPP_ */
