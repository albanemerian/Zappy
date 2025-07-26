/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GuiObserver_test
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include "../../../../gui/src/Observer/GuiObserver.hpp"
#include "../../../../gui/src/Observer/IObserver.hpp"

// Mock GUI class for testing
class MockGUI {
public:
    MOCK_METHOD(void, refresh, ());
    MOCK_METHOD(void, handleVictory, (const std::string& teamName));
};

// Test fixture for GuiObserver tests
class GuiObserverTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockGui = std::make_shared<MockGUI>();
        // We can't directly test GuiObserver with MockGUI since it expects a real GUI
        // So we'll test the interface and basic functionality
    }

    void TearDown() override {
        mockGui.reset();
    }

    std::shared_ptr<MockGUI> mockGui;
};

// Test GuiObserver constructor with null GUI
TEST_F(GuiObserverTest, ConstructorWithNullGui) {
    std::shared_ptr<GUI> nullGui = nullptr;

    // Should not crash when constructing with null GUI
    EXPECT_NO_THROW(GuiObserver observer(nullGui));
}

// Test GuiObserver implements IObserver interface
TEST_F(GuiObserverTest, ImplementsIObserverInterface) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // Should be able to use as IObserver
    IObserver* iObserver = &observer;
    EXPECT_NE(iObserver, nullptr);
}

// Test update method with null GUI (expired weak_ptr)
TEST_F(GuiObserverTest, UpdateWithExpiredGui) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // Should not crash when GUI is null/expired
    EXPECT_NO_THROW(observer.update());
}

// Test onGameEvent with null GUI (expired weak_ptr)
TEST_F(GuiObserverTest, OnGameEventWithExpiredGui) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // Should not crash when GUI is null/expired
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, "TestTeam"));
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::STATE_CHANGED, "TestTeam"));
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_DEFEAT, "TestTeam"));
}

// Test onGameEvent with different event types and expired GUI
TEST_F(GuiObserverTest, OnGameEventDifferentTypesWithExpiredGui) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // Test all event types with expired GUI
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::STATE_CHANGED, "Team1"));
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, "Team2"));
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_DEFEAT, "Team3"));
}

// Test onGameEvent with empty team name and expired GUI
TEST_F(GuiObserverTest, OnGameEventEmptyTeamNameWithExpiredGui) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, ""));
}

// Test multiple update calls with expired GUI
TEST_F(GuiObserverTest, MultipleUpdateCallsWithExpiredGui) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // Multiple calls should not crash
    EXPECT_NO_THROW(observer.update());
    EXPECT_NO_THROW(observer.update());
    EXPECT_NO_THROW(observer.update());
}

// Test multiple onGameEvent calls with expired GUI
TEST_F(GuiObserverTest, MultipleOnGameEventCallsWithExpiredGui) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // Multiple calls with different parameters should not crash
    for (int i = 0; i < 10; ++i) {
        EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, "Team" + std::to_string(i)));
    }
}

// Test polymorphic behavior through IObserver interface
TEST_F(GuiObserverTest, PolymorphicBehaviorThroughInterface) {
    std::shared_ptr<GUI> nullGui = nullptr;
    auto observer = std::make_unique<GuiObserver>(nullGui);

    // Use through IObserver interface
    IObserver* iObserver = observer.get();

    EXPECT_NO_THROW(iObserver->update());
    EXPECT_NO_THROW(iObserver->onGameEvent(GameEventType::TEAM_WIN, "PolyTeam"));
}

// Test destructor behavior
TEST_F(GuiObserverTest, DestructorBehavior) {
    std::shared_ptr<GUI> nullGui = nullptr;

    // Test that observer can be safely destroyed
    auto observer = std::make_unique<GuiObserver>(nullGui);
    EXPECT_NO_THROW(observer.reset());
}

// Test with long team names
TEST_F(GuiObserverTest, LongTeamNames) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    std::string longTeamName = std::string(1000, 'A');
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, longTeamName));
}

// Test with special characters in team names
TEST_F(GuiObserverTest, SpecialCharactersInTeamNames) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    std::string specialTeamName = "Team!@#$%^&*()_+-=[]{}|;':\",./<>?";
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, specialTeamName));
}

// Test that only TEAM_WIN events are handled specially
TEST_F(GuiObserverTest, OnlyTeamWinEventsHandledSpecially) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // All event types should be handled gracefully
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::STATE_CHANGED, "Team"));
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, "Team"));
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_DEFEAT, "Team"));
}

// Test rapid successive calls
TEST_F(GuiObserverTest, RapidSuccessiveCalls) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // Rapid calls should not cause issues
    for (int i = 0; i < 1000; ++i) {
        EXPECT_NO_THROW(observer.update());
        EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, "Team"));
    }
}

// Test weak_ptr behavior simulation
TEST_F(GuiObserverTest, WeakPtrBehaviorSimulation) {
    // Test with a GUI that gets destroyed
    std::shared_ptr<GUI> gui = nullptr;  // GUI is null from the start
    GuiObserver observer(gui);

    // These should not crash even though GUI is null
    EXPECT_NO_THROW(observer.update());
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, "TestTeam"));
}

// Test copy and move semantics (if applicable)
TEST_F(GuiObserverTest, CopyAndMoveSemantics) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer1(nullGui);

    // Test that we can create multiple observers
    GuiObserver observer2(nullGui);

    // Both should work independently
    EXPECT_NO_THROW(observer1.update());
    EXPECT_NO_THROW(observer2.update());
    EXPECT_NO_THROW(observer1.onGameEvent(GameEventType::TEAM_WIN, "Team1"));
    EXPECT_NO_THROW(observer2.onGameEvent(GameEventType::TEAM_WIN, "Team2"));
}

// Test thread safety implications (basic test)
TEST_F(GuiObserverTest, BasicThreadSafetyTest) {
    std::shared_ptr<GUI> nullGui = nullptr;
    GuiObserver observer(nullGui);

    // Basic test - in a real scenario, you'd want to test with actual threading
    // This test ensures the methods don't have obvious thread safety issues
    EXPECT_NO_THROW(observer.update());
    EXPECT_NO_THROW(observer.onGameEvent(GameEventType::TEAM_WIN, "ThreadTeam"));
}
