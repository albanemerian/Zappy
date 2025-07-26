/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** IObserver_test
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include "../../../../gui/src/Observer/IObserver.hpp"

// Concrete implementation of IObserver for testing
class ConcreteObserver : public IObserver {
public:
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, onGameEvent, (GameEventType eventType, const std::string& teamName), (override));
};

// Test fixture for IObserver tests
class IObserverTest : public ::testing::Test {
protected:
    void SetUp() override {
        observer = std::make_shared<ConcreteObserver>();
    }

    void TearDown() override {
        observer.reset();
    }

    std::shared_ptr<ConcreteObserver> observer;
};

// Test GameEventType enum values
TEST_F(IObserverTest, GameEventTypeEnumValues) {
    // Test that enum values exist and can be used
    GameEventType stateChanged = GameEventType::STATE_CHANGED;
    GameEventType teamWin = GameEventType::TEAM_WIN;
    GameEventType teamDefeat = GameEventType::TEAM_DEFEAT;

    // Test that enum values are distinct
    EXPECT_NE(stateChanged, teamWin);
    EXPECT_NE(stateChanged, teamDefeat);
    EXPECT_NE(teamWin, teamDefeat);
}

// Test that update method is pure virtual and can be mocked
TEST_F(IObserverTest, UpdateMethodIsPureVirtual) {
    EXPECT_CALL(*observer, update())
        .Times(1);

    observer->update();
}

// Test that onGameEvent has default implementation that does nothing
TEST_F(IObserverTest, OnGameEventDefaultImplementation) {
    // Create a real observer that uses the default implementation
    class DefaultObserver : public IObserver {
    public:
        void update() override {}
        // onGameEvent uses default implementation
    };

    DefaultObserver defaultObserver;

    // Should not crash when calling onGameEvent with default implementation
    EXPECT_NO_THROW(defaultObserver.onGameEvent(GameEventType::STATE_CHANGED, "TestTeam"));
    EXPECT_NO_THROW(defaultObserver.onGameEvent(GameEventType::TEAM_WIN, "WinningTeam"));
    EXPECT_NO_THROW(defaultObserver.onGameEvent(GameEventType::TEAM_DEFEAT, "LosingTeam"));
    EXPECT_NO_THROW(defaultObserver.onGameEvent(GameEventType::TEAM_WIN, ""));
}

// Test onGameEvent with STATE_CHANGED event
TEST_F(IObserverTest, OnGameEventStateChanged) {
    std::string teamName = "TestTeam";

    EXPECT_CALL(*observer, onGameEvent(GameEventType::STATE_CHANGED, teamName))
        .Times(1);

    observer->onGameEvent(GameEventType::STATE_CHANGED, teamName);
}

// Test onGameEvent with TEAM_WIN event
TEST_F(IObserverTest, OnGameEventTeamWin) {
    std::string teamName = "WinningTeam";

    EXPECT_CALL(*observer, onGameEvent(GameEventType::TEAM_WIN, teamName))
        .Times(1);

    observer->onGameEvent(GameEventType::TEAM_WIN, teamName);
}

// Test onGameEvent with TEAM_DEFEAT event
TEST_F(IObserverTest, OnGameEventTeamDefeat) {
    std::string teamName = "LosingTeam";

    EXPECT_CALL(*observer, onGameEvent(GameEventType::TEAM_DEFEAT, teamName))
        .Times(1);

    observer->onGameEvent(GameEventType::TEAM_DEFEAT, teamName);
}

// Test onGameEvent with empty team name
TEST_F(IObserverTest, OnGameEventEmptyTeamName) {
    std::string emptyTeamName = "";

    EXPECT_CALL(*observer, onGameEvent(GameEventType::TEAM_WIN, emptyTeamName))
        .Times(1);

    observer->onGameEvent(GameEventType::TEAM_WIN, emptyTeamName);
}

// Test multiple calls to update
TEST_F(IObserverTest, MultipleUpdateCalls) {
    EXPECT_CALL(*observer, update())
        .Times(3);

    observer->update();
    observer->update();
    observer->update();
}

// Test multiple calls to onGameEvent with different parameters
TEST_F(IObserverTest, MultipleOnGameEventCalls) {
    EXPECT_CALL(*observer, onGameEvent(GameEventType::STATE_CHANGED, "Team1"))
        .Times(1);
    EXPECT_CALL(*observer, onGameEvent(GameEventType::TEAM_WIN, "Team2"))
        .Times(1);
    EXPECT_CALL(*observer, onGameEvent(GameEventType::TEAM_DEFEAT, "Team3"))
        .Times(1);

    observer->onGameEvent(GameEventType::STATE_CHANGED, "Team1");
    observer->onGameEvent(GameEventType::TEAM_WIN, "Team2");
    observer->onGameEvent(GameEventType::TEAM_DEFEAT, "Team3");
}

// Test polymorphic behavior
TEST_F(IObserverTest, PolymorphicBehavior) {
    std::shared_ptr<IObserver> polymorphicObserver = observer;

    EXPECT_CALL(*observer, update())
        .Times(1);
    EXPECT_CALL(*observer, onGameEvent(GameEventType::TEAM_WIN, "PolyTeam"))
        .Times(1);

    polymorphicObserver->update();
    polymorphicObserver->onGameEvent(GameEventType::TEAM_WIN, "PolyTeam");
}

// Test destructor behavior
TEST_F(IObserverTest, DestructorBehavior) {
    // Test that observer can be safely destroyed
    auto tempObserver = std::make_shared<ConcreteObserver>();

    EXPECT_NO_THROW(tempObserver.reset());
}

// Test with long team names
TEST_F(IObserverTest, LongTeamNames) {
    std::string longTeamName = std::string(1000, 'A');

    EXPECT_CALL(*observer, onGameEvent(GameEventType::TEAM_WIN, longTeamName))
        .Times(1);

    observer->onGameEvent(GameEventType::TEAM_WIN, longTeamName);
}

// Test with special characters in team names
TEST_F(IObserverTest, SpecialCharactersInTeamNames) {
    std::string specialTeamName = "Team!@#$%^&*()_+-=[]{}|;':\",./<>?";

    EXPECT_CALL(*observer, onGameEvent(GameEventType::TEAM_WIN, specialTeamName))
        .Times(1);

    observer->onGameEvent(GameEventType::TEAM_WIN, specialTeamName);
}
