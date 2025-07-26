/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Camera_manager_test
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <cmath>
#include "../../../../gui/src/Graphic/Camera/CameraManager.hpp"
#include "../../../../gui/src/Game/GameInfos.hpp"
#include "../../../../gui/src/Graphic/Map.hpp"

class MockDisplay : public IDisplay {
public:
    MOCK_METHOD(Vector2i, getMonitorSize, (), (override));
    MOCK_METHOD(Vector2i, getScreenSize, (), (override));
    MOCK_METHOD(void, initWindow, (int width, int height, std::string title), (override));
    MOCK_METHOD(void, initCamera, (), (override));
    MOCK_METHOD(bool, isWindowReady, (), (override));
    MOCK_METHOD(void, setTargetFPS, (unsigned int FPS), (override));
    MOCK_METHOD(bool, isOpen, (), (override));
    MOCK_METHOD(void, closeWindow, (), (override));
    MOCK_METHOD(int, getKeyId, (enum Key), (override));
    MOCK_METHOD(bool, isKeyReleased, (int key), (override));
    MOCK_METHOD(bool, isKeyPressed, (int key), (override));
    MOCK_METHOD(bool, isKeyDown, (int key), (override));
    MOCK_METHOD(bool, isGamepadAvailable, (), (override));
    MOCK_METHOD(bool, isGamepadButtonReleased, (int key), (override));
    MOCK_METHOD(bool, isGamepadButtonPressed, (int key), (override));
    MOCK_METHOD(bool, isGamepadButtonDown, (int key), (override));
    MOCK_METHOD(bool, isMouseButtonDown, (int key), (override));
    MOCK_METHOD(bool, isMouseButtonReleased, (int key), (override));
    MOCK_METHOD(bool, isMouseButtonPressed, (int key), (override));
    MOCK_METHOD(Vector2f, getMousePosition, (), (override));
    MOCK_METHOD(void, setMousePosition, (Vector2f), (override));
    MOCK_METHOD(float, getMouseWheelMove, (), (override));
    MOCK_METHOD(float, getGamepadAxisMovement, (int key), (override));
    MOCK_METHOD(void, setCameraPosition, (Vector3f), (override));
    MOCK_METHOD(void, setCameraTarget, (Vector3f), (override));
    MOCK_METHOD(Vector2f, getMouseDelta, (), (override));
    MOCK_METHOD(float, vector3DDistanceFromCamera, (Vector3f target), (override));
    MOCK_METHOD(Vector3f, vector3SubtractFromCamera, (Vector3f target), (override));
    MOCK_METHOD(Vector3f, vector3Normalize, (Vector3f), (override));
    MOCK_METHOD(void, enableCursor, (), (override));
    MOCK_METHOD(void, disableCursor, (), (override));
    MOCK_METHOD(float, getFrameTime, (), (override));
    MOCK_METHOD(int, getFPS, (), (override));
    MOCK_METHOD(void, updateCameraFreeMode, (float camMovingSpeed, float camRotaSpeed), (override));
    MOCK_METHOD(InputType, getLastInputType, (), (const, override));
    MOCK_METHOD(void, updateLastInputType, (), (override));
    MOCK_METHOD(float, measureText, (const std::string& text, float fontSize), (const, override));
    MOCK_METHOD(bool, checkCollisionPointRec, (Vector2f point, FloatRect rec), (override));
    MOCK_METHOD(Ray3D, getMouseRay, (Vector2f mousePosition), (override));
    MOCK_METHOD(RayCollision3D, getRayCollisionBox, (Ray3D ray, BoundingBox3D box), (override));
    MOCK_METHOD(RayCollision3D, getRayCollisionSphere, (Ray3D ray, Vector3f center, float radius), (override));
    MOCK_METHOD(bool, checkCollisionBoxes, (BoundingBox3D box1, BoundingBox3D box2), (override));
    MOCK_METHOD(Ray3D, getMouseRayFromCurrent, (), (override));
    MOCK_METHOD(BoundingBox3D, createBoundingBox, (Vector3f center, Vector3f size), (override));
    MOCK_METHOD(BoundingBox3D, createBoundingBoxFromMinMax, (Vector3f min, Vector3f max), (override));
    MOCK_METHOD(void, beginDrawing, (), (override));
    MOCK_METHOD(void, endDrawing, (), (override));
    MOCK_METHOD(void, clearBackground, (Color32), (override));
    MOCK_METHOD(void, begin3DMode, (), (override));
    MOCK_METHOD(void, end3DMode, (), (override));
    MOCK_METHOD(void, endScissorMode, (), (override));
    MOCK_METHOD(void, beginScissorMode, (IntRect), (override));
    MOCK_METHOD(bool, loadModel, (const std::string& id, const std::string& filepath, Vector3f center), (override));
    MOCK_METHOD(void, drawCube, (Vector3f position, float width, float height, float length, Color32 color), (override));
    MOCK_METHOD(void, drawCubeWires, (Vector3f position, float width, float height, float length, Color32 color), (override));
    MOCK_METHOD(void, drawSphere, (Vector3f position, float radius, Color32 color), (override));
    MOCK_METHOD(void, drawSphereWires, (Vector3f position, float radius, int rings, int slices, Color32 color), (override));
    MOCK_METHOD(void, drawCylinder, (Vector3f position, float radiusTop, float radiusBottom, float height, int slices, Color32 color), (override));
    MOCK_METHOD(void, drawCylinderWires, (Vector3f position, float radiusTop, float radiusBottom, float height, int slices, Color32 color), (override));
    MOCK_METHOD(void, drawCylinderEx, (Vector3f startPos, Vector3f endPos, float startRadius, float endRadius, int sides, Color32 color), (override));
    MOCK_METHOD(void, drawPlane, (Vector3f position, Vector2f size, Color32 color), (override));
    MOCK_METHOD(void, drawLine3D, (Vector3f startPos, Vector3f endPos, Color32 color), (override));
    MOCK_METHOD(void, drawModelEx, (const std::string& id, Vector3f position, Vector3f rotationAxis, float rotationAngle, Vector3f scale, Color32 tint), (override));
    MOCK_METHOD(void, drawCircle, (float centerX, float centerY, float radius, Color32 color), (override));
    MOCK_METHOD(void, drawCircleLines, (float centerX, float centerY, float radius, Color32 color), (override));
    MOCK_METHOD(void, drawText, (const std::string& text, float x, float y, float fontSize, Color32 color), (override));
    MOCK_METHOD(void, drawTextEx, (const std::string& text, float x, float y, float fontSize, float spacing, Color32 color), (override));
    MOCK_METHOD(void, drawRectangleRec, (FloatRect rec, Color32 color), (override));
    MOCK_METHOD(bool, loadTexture, (const std::string& id, const std::string& filepath), (override));
    MOCK_METHOD(bool, loadFont, (const std::string& id, const std::string& filepath), (override));
    MOCK_METHOD(void, drawTexture, (const std::string& id, float x, float y, Color32 tint), (override));
    MOCK_METHOD(void, drawTextureScaled, (const std::string& id, float x, float y, float width, float height, Color32 tint), (override));
    MOCK_METHOD(Vector2f, getTextureSize, (const std::string& id), (const, override));
    MOCK_METHOD(bool, loadSkybox, (const std::string& id, const std::string& filepath), (override));
    MOCK_METHOD(void, drawSkybox, (const std::string& id), (override));
    MOCK_METHOD(float, getTime, (), (const, override));
};

class MockGameInfos : public GameInfos {
public:
    MockGameInfos() : GameInfos(nullptr) {}
    MOCK_METHOD(const std::vector<zappy::structs::Player>&, getPlayers, (), (const));
    MOCK_METHOD(bool, isTeamVisible, (const std::string& teamName), (const));
};

class MockMap : public Map {
public:
    MockMap() : Map(nullptr, nullptr) {}
    MOCK_METHOD(Vector3f, getPlayerInterpolatedPosition, (int playerNumber, int x, int y));
    MOCK_METHOD(float, getOffset, (DisplayPriority priority, int x, int y, size_t index));
};

class CameraManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockDisplay = std::make_shared<testing::NiceMock<MockDisplay>>();
        mockGameInfos = std::make_shared<testing::NiceMock<MockGameInfos>>();
        mockMap = std::make_shared<testing::NiceMock<MockMap>>();
        cameraManager = std::make_unique<CameraManager>(mockDisplay);

        testPlayersList.clear();
        emptyPlayersList.clear();

        ON_CALL(*mockDisplay, getScreenSize())
            .WillByDefault(testing::Return(Vector2i{1920, 1080}));
        ON_CALL(*mockDisplay, getFrameTime())
            .WillByDefault(testing::Return(0.016f));
    }

    void TearDown() override {
        cameraManager.reset();
        mockDisplay.reset();
        mockGameInfos.reset();
        mockMap.reset();
    }

    std::unique_ptr<CameraManager> cameraManager;
    std::shared_ptr<testing::NiceMock<MockDisplay>> mockDisplay;
    std::shared_ptr<testing::NiceMock<MockGameInfos>> mockGameInfos;
    std::shared_ptr<testing::NiceMock<MockMap>> mockMap;

    std::vector<zappy::structs::Player> testPlayersList;
    std::vector<zappy::structs::Player> emptyPlayersList;
};

TEST_F(CameraManagerTest, ConstructorInitializesCorrectly) {
    EXPECT_NE(cameraManager, nullptr);
    EXPECT_EQ(cameraManager->getPlayerId(), -1);
    EXPECT_EQ(cameraManager->getCameraMovingSpeed(), 15.0f);
    EXPECT_EQ(cameraManager->getCameraRotaSpeed(), 2.0f);
    EXPECT_EQ(cameraManager->getCameraZoomSpeed(), 120.0f);
}

TEST_F(CameraManagerTest, SetAndGetPlayerIdWorksCorrectly) {
    cameraManager->setPlayerId(42);
    EXPECT_EQ(cameraManager->getPlayerId(), 42);

    cameraManager->setPlayerId(-1);
    EXPECT_EQ(cameraManager->getPlayerId(), -1);

    cameraManager->setPlayerId(0);
    EXPECT_EQ(cameraManager->getPlayerId(), 0);
}

TEST_F(CameraManagerTest, SetAndGetCameraSpeedsWorkCorrectly) {
    cameraManager->setCameraMovingSpeed(25.0f);
    EXPECT_FLOAT_EQ(cameraManager->getCameraMovingSpeed(), 25.0f);

    cameraManager->setCameraRotaSpeed(5.0f);
    EXPECT_FLOAT_EQ(cameraManager->getCameraRotaSpeed(), 5.0f);

    cameraManager->setCameraZoomSpeed(200.0f);
    EXPECT_FLOAT_EQ(cameraManager->getCameraZoomSpeed(), 200.0f);
}

TEST_F(CameraManagerTest, SetGameInfosWorksCorrectly) {
    EXPECT_NO_THROW(cameraManager->setGameInfos(mockGameInfos));
}

TEST_F(CameraManagerTest, SetMapInstanceWorksCorrectly) {
    EXPECT_NO_THROW(cameraManager->setMapInstance(mockMap));
}

TEST_F(CameraManagerTest, SetMapCenterWorksCorrectly) {
    Vector3f center = {10.0f, 5.0f, 15.0f};
    EXPECT_NO_THROW(cameraManager->setMapCenter(center));
}

TEST_F(CameraManagerTest, SetMapSizeWorksCorrectly) {
    EXPECT_NO_THROW(cameraManager->setMapSize(20, 30));
}

TEST_F(CameraManagerTest, SetTargetDistanceClampsBounds) {
    cameraManager->setTargetDistance(3.0f);

    cameraManager->setTargetDistance(150.0f);

    cameraManager->setTargetDistance(25.0f);
}

TEST_F(CameraManagerTest, InitTargetPositionFromCurrentCameraWorksCorrectly) {
    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    Vector3f scaledMapCenter = {10.0f * 2.2f, 0.0f, 10.0f * 2.2f}; // POSITION_MULTIPLIER = 2.2f
    Vector3f direction = {1.0f, 0.5f, 0.5f};
    float distance = 15.0f;

    EXPECT_CALL(*mockDisplay, vector3DDistanceFromCamera(scaledMapCenter))
        .WillOnce(testing::Return(distance));
    EXPECT_CALL(*mockDisplay, vector3SubtractFromCamera(scaledMapCenter))
        .WillOnce(testing::Return(direction));
    EXPECT_CALL(*mockDisplay, vector3Normalize(direction))
        .WillOnce(testing::Return(Vector3f{0.8f, 0.4f, 0.4f}));

    cameraManager->setMapCenter(mapCenter);
    EXPECT_NO_THROW(cameraManager->initTargetPositionFromCurrentCamera());
}

TEST_F(CameraManagerTest, UpdateCameraFreeModeCallsDisplay) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->updateCameraFreeMode();
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeWithMouseInput) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*mockDisplay, getScreenSize()).WillRepeatedly(testing::Return(Vector2i{1920, 1080}));
    EXPECT_CALL(*mockDisplay, setMousePosition(testing::_)).Times(2); // Called twice: once on press, once during dragging
    EXPECT_CALL(*mockDisplay, disableCursor()).Times(1);
    EXPECT_CALL(*mockDisplay, getMouseDelta()).WillOnce(testing::Return(Vector2f{5.0f, -3.0f}));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, isKeyDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);
    cameraManager->updateCameraTargetMode();
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeWithMouseRelease) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*mockDisplay, enableCursor()).Times(1);
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, isKeyDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);
    cameraManager->updateCameraTargetMode();
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeWithMouseWheel) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(2.0f));
    EXPECT_CALL(*mockDisplay, isKeyDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);
    cameraManager->updateCameraTargetMode();
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeWithKeyboardInput) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, getKeyId(testing::_)).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(*mockDisplay, isKeyDown(1))
        .WillOnce(testing::Return(true))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);
    cameraManager->updateCameraTargetMode();
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeWithGamepad) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, isKeyDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(true));
    EXPECT_CALL(*mockDisplay, getKeyId(testing::_)).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(*mockDisplay, getGamepadAxisMovement(1))
        .WillOnce(testing::Return(0.8f))
        .WillOnce(testing::Return(-0.6f));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);
    cameraManager->updateCameraTargetMode();
}

TEST_F(CameraManagerTest, UpdateCameraPlayerModeWithNoGameInfos) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(2); // Called in updateCameraPlayerMode and updateCameraFreeMode
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->updateCameraPlayerMode();
}

TEST_F(CameraManagerTest, UpdateCameraPlayerModeWithInvalidPlayerId) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(2); // Called in updateCameraPlayerMode and updateCameraFreeMode
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->setGameInfos(mockGameInfos);
    cameraManager->setPlayerId(-1);
    cameraManager->updateCameraPlayerMode();
}

TEST_F(CameraManagerTest, UpdateCameraPlayerModeWithPlayerNotFound) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(2); // Called in updateCameraPlayerMode and updateCameraFreeMode
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->setGameInfos(mockGameInfos);
    cameraManager->setMapInstance(mockMap);
    cameraManager->setPlayerId(1);
    cameraManager->updateCameraPlayerMode();
}

TEST_F(CameraManagerTest, UpdateCameraPlayerModeWithInvisibleTeam) {
    zappy::structs::Player testPlayer;
    testPlayer.number = 1;
    testPlayer.x = 5;
    testPlayer.y = 10;
    testPlayer.teamName = "team1";
    testPlayersList = {testPlayer};

    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(2); // Called in updateCameraPlayerMode and updateCameraFreeMode
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->setGameInfos(mockGameInfos);
    cameraManager->setMapInstance(mockMap);
    cameraManager->setPlayerId(1);
    cameraManager->updateCameraPlayerMode();
}

TEST_F(CameraManagerTest, UpdateCameraPlayerModeWithKeyboardRotation) {
    zappy::structs::Player testPlayer;
    testPlayer.number = 1;
    testPlayer.x = 5;
    testPlayer.y = 10;
    testPlayer.teamName = "team1";
    testPlayersList = {testPlayer};

    cameraManager->setGameInfos(mockGameInfos);
    cameraManager->setMapInstance(mockMap);
    cameraManager->setPlayerId(1);
    cameraManager->updateCameraPlayerMode();
    SUCCEED();
}

TEST_F(CameraManagerTest, UpdateCameraWithFreeModeEnum) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->updateCamera(zappy::gui::CameraMode::FREE);
}

TEST_F(CameraManagerTest, UpdateCameraWithTargetModeEnum) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, isKeyDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);
    cameraManager->updateCamera(zappy::gui::CameraMode::TARGETED);
}

TEST_F(CameraManagerTest, UpdateCameraWithPlayerModeEnum) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(2); // Called in updateCameraPlayerMode and updateCameraFreeMode
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->updateCamera(zappy::gui::CameraMode::PLAYER);
}

TEST_F(CameraManagerTest, CalculatePlayerPositionWithoutMap) {
    zappy::structs::Player testPlayer;
    testPlayer.x = 5;
    testPlayer.y = 10;
    testPlayer.number = 1;

    Vector3f result = cameraManager->calculatePlayerPosition(testPlayer);

    EXPECT_FLOAT_EQ(result.x, 5.0f * zappy::gui::POSITION_MULTIPLIER);
    EXPECT_FLOAT_EQ(result.y, 0.7f);
    EXPECT_FLOAT_EQ(result.z, 10.0f * zappy::gui::POSITION_MULTIPLIER);
}

TEST_F(CameraManagerTest, CalculateCameraPositionWorksCorrectly) {
    Vector3f playerPos = {10.0f, 5.0f, 15.0f};
    float angleXZ = M_PI / 4;

    Vector3f result = cameraManager->calculateCameraPosition(playerPos, angleXZ);

    const float fixedAngleY = 0.5f;
    const float fixedDistance = 6.0f;

    float expectedX = playerPos.x + fixedDistance * cosf(fixedAngleY) * cosf(angleXZ);
    float expectedY = playerPos.y + fixedDistance * sinf(fixedAngleY);
    float expectedZ = playerPos.z + fixedDistance * cosf(fixedAngleY) * sinf(angleXZ);

    EXPECT_NEAR(result.x, expectedX, 0.01f);
    EXPECT_NEAR(result.y, expectedY, 0.01f);
    EXPECT_NEAR(result.z, expectedZ, 0.01f);
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeWithGamepadZoomIn) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, isKeyDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getKeyId(testing::_)).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(1))
        .WillOnce(testing::Return(true))
        .WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);
    cameraManager->updateCameraTargetMode();
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeWithGamepadZoomOut) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, isKeyDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getKeyId(testing::_)).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(1))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(true));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);
    cameraManager->updateCameraTargetMode();
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeWithDragging) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getScreenSize()).WillRepeatedly(testing::Return(Vector2i{1920, 1080}));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, isKeyDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);

    cameraManager->updateCameraTargetMode();
    SUCCEED();
}

TEST_F(CameraManagerTest, UpdateCameraTargetModeAllKeyboardDirections) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(4);
    EXPECT_CALL(*mockDisplay, getFrameTime()).WillRepeatedly(testing::Return(0.016f));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove()).WillRepeatedly(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, getKeyId(testing::_)).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(*mockDisplay, isGamepadAvailable()).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isGamepadButtonDown(testing::_)).WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(4);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(4);

    Vector3f mapCenter = {10.0f, 0.0f, 10.0f};
    cameraManager->setMapCenter(mapCenter);

    EXPECT_CALL(*mockDisplay, isKeyDown(1))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(true))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(false));
    cameraManager->updateCameraTargetMode();

    EXPECT_CALL(*mockDisplay, isKeyDown(1))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(true))
        .WillOnce(testing::Return(false));
    cameraManager->updateCameraTargetMode();

    EXPECT_CALL(*mockDisplay, isKeyDown(1))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(true));
    cameraManager->updateCameraTargetMode();

    EXPECT_CALL(*mockDisplay, isKeyDown(1))
        .WillOnce(testing::Return(true))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(false))
        .WillOnce(testing::Return(false));
    cameraManager->updateCameraTargetMode();
}

TEST_F(CameraManagerTest, SetTargetDistanceWithBoundaryValues) {
    cameraManager->setTargetDistance(4.0f);

    cameraManager->setTargetDistance(101.0f);

    cameraManager->setTargetDistance(5.0f);

    cameraManager->setTargetDistance(100.0f);

    cameraManager->setTargetDistance(50.0f);
}

TEST_F(CameraManagerTest, UpdateCameraWithInvalidMode) {
    EXPECT_CALL(*mockDisplay, updateLastInputType()).Times(1);
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->updateCamera(static_cast<zappy::gui::CameraMode>(999));
}
