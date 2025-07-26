/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RayLibEnc
*/

#ifndef RAYLIBENC_HPP_
#define RAYLIBENC_HPP_

#include <string>
#include <map>
#include <memory>
#include "raylib.h"
#include "../../Utils/InputType.hpp"

class RayLibEnc {
    public:
        RayLibEnc();
        ~RayLibEnc();

        // Window management methods
        void initWindow(int width, int height, const std::string &title);
        void closeWindow();
        bool windowShouldClose() const;
        void beginDrawing();
        void endDrawing();
        void clearBackground(Color color = WHITE);
        bool isWindowReady() const;
        int getMonitorWidth(int monitor) const;
        int getMonitorHeight(int monitor) const;
        void waitTime(float seconds) const;
        void setTargetFPS(int fps) const;
        int getFPS() const;
        float getFrameTime() const;

        // Collision methods
        bool checkCollisionPointRec(Vector2 point, Rectangle rec) const;

        // Ray and collision methods for 3D
        Ray getMouseRay(Vector2 mousePosition);
        RayCollision getRayCollisionBox(Ray ray, BoundingBox box);
        RayCollision getRayCollisionSphere(Ray ray, Vector3 center, float radius);
        bool checkCollisionBoxes(BoundingBox box1, BoundingBox box2);

        // Utility methods for 3D collisions
        Ray getMouseRayFromCurrent();
        BoundingBox createBoundingBox(Vector3 center, Vector3 size);
        BoundingBox createBoundingBoxFromMinMax(Vector3 min, Vector3 max);

        // Texture methods
        void drawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
        void unloadTexture(Texture2D texture);
        Texture2D loadTextureFromFile(const std::string& filepath);
        void drawTextureEx(Texture2D texture, Vector2 position, Color tint);
        void drawTextureScaled(Texture2D texture, float x, float y, float width, float height,
            Color tint);

        // Texture map accessor methods
        bool hasTexture(const std::string& id) const;
        Texture2D getTexture(const std::string& id) const;
        void addTexture(const std::string& id, Texture2D texture);

        // Input methods
        bool isMouseButtonDown(int button) const;
        bool isMouseButtonPressed(int button) const;
        bool isMouseButtonReleased(int button) const;
        bool isKeyDown(int key) const;
        bool isKeyPressed(int key) const;
        bool isKeyReleased(int key) const;
        Vector2 getMouseDelta();
        Vector2 getMousePosition() const;
        void setMousePosition(int x, int y);
        void disableCursor();
        void enableCursor();
        int getScreenWidth() const;
        int getScreenHeight() const;
        float getMouseWheelMove() const;

        // Gamepad methods
        bool isGamepadAvailable(int gamepad) const;
        bool isGamepadButtonPressed(int gamepad, int button) const;
        bool isGamepadButtonDown(int gamepad, int button) const;
        bool isGamepadButtonReleased(int gamepad, int button) const;
        float getGamepadAxisMovement(int gamepad, int axis) const;

        // Input type tracking methods
        InputType getLastInputType() const;
        void updateLastInputType();

        // Scissor mode methods for clipping
        void beginScissorMode(int x, int y, int width, int height);
        void endScissorMode();

        // 3D Environment methods
        void begin3DMode();
        void end3DMode();
        float vector3Distance(Vector3 v1, Vector3 v2) const;
        Vector3 vector3Normalize(Vector3 v) const;
        Vector3 vector3Subtract(Vector3 v1, Vector3 v2) const;
        Vector3 vector3Add(Vector3 v1, Vector3 v2) const;

        // Camera methods
        void initCamera();
        void setCameraPosition(Vector3 position);
        void setCameraTarget(Vector3 target);
        void setCameraUp(Vector3 up);
        void setCameraFovy(float fovy);
        void setCameraProjection(int projection);
        void updateCamera(int mode = CAMERA_FREE);
        void updateCameraFreeMode(float camMovingSpeed, float camRotaSpeed);
        Camera3D getCamera() const;

        // 3D Drawing methods
        void drawGrid(int slices, float spacing);
        void drawCube(Vector3 position, float width, float height, float length, Color color);
        void drawCubeWires(Vector3 position, float width, float height, float length,
            Color color);
        void drawSphere(Vector3 position, float radius, Color color);
        void drawSphereWires(Vector3 position, float radius, int rings, int slices,
            Color color);
        void drawCylinder(Vector3 position, float radiusTop, float radiusBottom,
            float height, int slices, Color color);
        void drawCylinderWires(Vector3 position, float radiusTop, float radiusBottom,
            float height, int slices, Color color);
        void drawCylinderEx(Vector3 startPos, Vector3 endPos, float startRadius,
            float endRadius, int sides, Color color);
        void drawPlane(Vector3 position, Vector2 size, Color color);
        void drawLine3D(Vector3 startPos, Vector3 endPos, Color color);

        // 3D Model methods
        bool loadModel(const std::string& id, const std::string& filepath,
            Vector3 center = {0.0f, 0.0f, 0.0f});
        void drawModel(const std::string& id, Vector3 position, float scale,
            Color tint = WHITE);
        void drawModelEx(const std::string& id, Vector3 position, Vector3 rotationAxis,
                         float rotationAngle, Vector3 scale, Color tint = WHITE);
        void drawModelWires(const std::string& id, Vector3 position, float scale,
            Color tint = WHITE);
        void drawModelWiresEx(const std::string& id, Vector3 position, Vector3 rotationAxis,
                              float rotationAngle, Vector3 scale, Color tint = WHITE);
        void unloadModel(const std::string& id);
        void unloadAllModels();
        bool modelExists(const std::string& id) const;

        // Skybox methods
        bool loadSkybox(const std::string& id, const std::string& filepath);
        void drawSkybox(const std::string& id);
        Color getDayNightColor(float cycleTime);
        float getTime() const;

        // 2D Drawing methods
        void drawRectangleRec(Rectangle rec, Color color);
        void drawText(const std::string& text, float x, float y, float fontSize, Color color);
        void drawTextEx(const std::string& text, float x, float y, float fontSize,
            float spacing, Color color);
        void drawCircle(float centerX, float centerY, float radius, Color color);
        void drawCircleLines(float centerX, float centerY, float radius, Color color);
        float measureText(const std::string& text, float fontSize) const;
        float measureTextEx(const std::string& text, float fontSize, float spacing) const;

        // Font methods
        bool loadFont(const std::string& id, const std::string& filepath);
        void unloadFont(const std::string& id);
        bool hasFontLoaded(const std::string& id) const;
        Font getFont(const std::string& id) const;
        void unloadAllFonts();

    private:
        bool _isInitialized;
        Camera3D _camera;
        Vector2 _previousMousePosition;
        bool _isCursorLocked;
        InputType _lastInputType;

        static constexpr float FONT_SCALE_FACTOR = 4.0f;
        static constexpr float FONT_RENDER_SCALE = 0.25f;
        static constexpr float FONT_SPACING_RATIO = 0.1f;

        float getScaledFontSize(float fontSize) const;
        float getFontSpacing(float scaledFontSize) const;
        float getScaledSpacing(float spacing) const;

        struct ModelData {
            Model model;
            unsigned int animationCount;
            Vector3 center;
        };

        std::map<std::string, ModelData> _models;
        std::map<std::string, Texture2D> _textures;
        std::map<std::string, Sound> _sounds;
        std::map<std::string, Music> _musics;
        std::map<std::string, Font> _fonts;
};

#endif /* !RAYLIBEnc_HPP_ */
