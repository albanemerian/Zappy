/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** IDisplay
*/

#ifndef IDISPLAY_HPP_
#define IDISPLAY_HPP_
#include <utility>
#include <string>
#include "Utils/InputType.hpp"

enum Key {
    TAB,
    ESC,
    UP,
    DOWN,
    RIGHT,
    LEFT,
    H,
    C,
    GM_PD_LEFT_SHOULDER,
    GM_PD_RIGHT_SHOULDER,
    GM_PD_LEFT_TRIGGER,
    GM_PD_RIGHT_TRIGGER,
    GM_PD_UP,
    GM_PD_DOWN,
    GM_PD_AXIS_RIGHT_X,
    GM_PD_AXIS_RIGHT_Y,
    GM_PD_H,
    MOUSE_LEFT,
    MOUSE_RIGHT,
};

typedef struct Vector3f {
    float x;
    float y;
    float z;

    bool operator==(const Vector3f& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3f& other) const {
        return !(*this == other);
    }
} Vector3f;

typedef struct Vector2f {
    float x;
    float y;
} Vector2f;

typedef struct Vector2i {
    int x;
    int y;
} Vector2i;

typedef struct Color32 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color32;

typedef struct FloatRect {
    float x;
    float y;
    float width;
    float height;
} FloatRect;

typedef struct IntRect {
    int x;
    int y;
    int width;
    int height;
} IntRect;

typedef struct Ray3D {
    Vector3f position;
    Vector3f direction;
} Ray3D;

typedef struct RayCollision3D {
    bool hit;
    float distance;
    Vector3f point;
    Vector3f normal;
} RayCollision3D;

typedef struct BoundingBox3D {
    Vector3f min;
    Vector3f max;
} BoundingBox3D;

#define COLOR(r, g, b) Color32{ r, g, b, 255 }
#define CLIGHTGRAY COLOR(200, 200, 200)
#define CBLACK COLOR(0, 0, 0)
#define CRED COLOR(230, 41, 55)
#define CBROWN COLOR(127, 106, 79)
#define CBLUE COLOR(0, 121, 241)
#define CWHITE COLOR(255, 255, 255)

#define CRAYWHITE COLOR(245, 245, 245)
#define CPINK COLOR(255, 109, 194)
#define CGREEN COLOR(0, 228, 48)
#define CMAROON COLOR(190, 33, 55)
#define CPURPLE COLOR(200, 122, 255)
#define CORANGE COLOR(255, 161, 0)
#define CYELLOW COLOR(253, 249, 0)

class IDisplay {
    public:
        virtual Vector2i getMonitorSize() = 0;
        virtual Vector2i getScreenSize() = 0;

        virtual void initWindow(int width, int height, std::string) = 0;
        virtual void initCamera() = 0;

        virtual bool isWindowReady() = 0;
        virtual void setTargetFPS(unsigned int FPS) = 0;

        virtual bool isOpen() = 0;
        virtual void closeWindow() = 0;

        virtual int getKeyId(enum Key) = 0;

        virtual bool isKeyReleased(int key) = 0;
        virtual bool isKeyPressed(int key) = 0;
        virtual bool isKeyDown(int key) = 0;

        virtual bool isGamepadAvailable() = 0;

        virtual bool isGamepadButtonReleased(int key) = 0;
        virtual bool isGamepadButtonPressed(int key) = 0;
        virtual bool isGamepadButtonDown(int key) = 0;

        virtual bool isMouseButtonDown(int key) = 0;
        virtual bool isMouseButtonReleased(int key) = 0;
        virtual bool isMouseButtonPressed(int key) = 0;

        virtual Vector2f getMousePosition() = 0;
        virtual void setMousePosition(Vector2f) = 0;

        virtual float getMouseWheelMove() = 0;

        virtual float getGamepadAxisMovement(int key) = 0;

        virtual void setCameraPosition(Vector3f) = 0;

        virtual void setCameraTarget(Vector3f) = 0;

        virtual Vector2f getMouseDelta() = 0;

        virtual float vector3DDistanceFromCamera(Vector3f target) = 0;
        virtual Vector3f vector3SubtractFromCamera(Vector3f target) = 0;

        virtual Vector3f vector3Normalize(Vector3f) = 0;


        virtual void enableCursor() = 0;
        virtual void disableCursor() = 0;

        virtual float getFrameTime() = 0;
        virtual int getFPS() = 0;

        virtual void updateCameraFreeMode(float camMovingSpeed, float camRotaSpeed) = 0;

        virtual InputType getLastInputType() const = 0;
        virtual void updateLastInputType() = 0;

        virtual float measureText(const std::string& text, float fontSize) const = 0;

        virtual bool checkCollisionPointRec(Vector2f point, FloatRect rec) = 0;

        virtual Ray3D getMouseRay(Vector2f mousePosition) = 0;
        virtual RayCollision3D getRayCollisionBox(Ray3D ray, BoundingBox3D box) = 0;
        virtual RayCollision3D getRayCollisionSphere(Ray3D ray, Vector3f center,
            float radius) = 0;
        virtual bool checkCollisionBoxes(BoundingBox3D box1, BoundingBox3D box2) = 0;

        virtual Ray3D getMouseRayFromCurrent() = 0;
        virtual BoundingBox3D createBoundingBox(Vector3f center, Vector3f size) = 0;
        virtual BoundingBox3D createBoundingBoxFromMinMax(Vector3f min, Vector3f max) = 0;

        virtual void beginDrawing() = 0;
        virtual void endDrawing() = 0;
        virtual void clearBackground(Color32) = 0;

        virtual void begin3DMode() = 0;
        virtual void end3DMode() = 0;

        virtual void endScissorMode() = 0;
        virtual void beginScissorMode(IntRect) = 0;

        virtual bool loadModel(const std::string& id, const std::string& filepath,
            Vector3f center = {0.0f, 0.0f, 0.0f}) = 0;

        virtual void drawCube(Vector3f position, float width, float height, float length,
            Color32 color) = 0;
        virtual void drawCubeWires(Vector3f position, float width, float height, float length,
            Color32 color) = 0;

        virtual void drawSphere(Vector3f position, float radius, Color32 color) = 0;
        virtual void drawSphereWires(Vector3f position, float radius, int rings, int slices,
            Color32 color) = 0;

        virtual void drawCylinder(Vector3f position, float radiusTop, float radiusBottom,
            float height, int slices, Color32 color) = 0;
        virtual void drawCylinderWires(Vector3f position, float radiusTop, float radiusBottom,
            float height, int slices, Color32 color) = 0;
        virtual void drawCylinderEx(Vector3f startPos, Vector3f endPos, float startRadius,
            float endRadius, int sides, Color32 color) = 0;

        virtual void drawPlane(Vector3f position, Vector2f size, Color32 color) = 0;

        virtual void drawLine3D(Vector3f startPos, Vector3f endPos, Color32 color) = 0;

        virtual void drawModelEx(const std::string& id, Vector3f position,
            Vector3f rotationAxis, float rotationAngle, Vector3f scale,
            Color32 tint = CWHITE) = 0;

        virtual void drawCircle(float centerX, float centerY, float radius,
            Color32 color) = 0;
        virtual void drawCircleLines(float centerX, float centerY, float radius,
            Color32 color) = 0;

        virtual void drawText(const std::string& text, float x, float y, float fontSize,
            Color32 color) = 0;

        virtual void drawTextEx(const std::string& text, float x, float y, float fontSize,
            float spacing, Color32 color) = 0;

        virtual void drawRectangleRec(FloatRect rec, Color32 color) = 0;

        virtual bool loadTexture(const std::string& id, const std::string& filepath) = 0;

        virtual bool loadFont(const std::string& id, const std::string& filepath) = 0;

        virtual void drawTexture(const std::string& id, float x, float y,
            Color32 tint = CWHITE) = 0;

        virtual void drawTextureScaled(const std::string& id, float x, float y, float width,
            float height, Color32 tint = CWHITE) = 0;

        virtual Vector2f getTextureSize(const std::string& id) const = 0;

        virtual bool loadSkybox(const std::string& id, const std::string& filepath) = 0;

        virtual void drawSkybox(const std::string& id) = 0;

        virtual float getTime() const = 0;

        ~IDisplay() = default;
};

#endif /* !IDISPLAY_HPP_ */
