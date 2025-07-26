/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Subject_test
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include "../../../../gui/src/Observer/Subject.hpp"
#include "../../../../gui/src/Observer/IObserver.hpp"

// Mock Observer for testing
class MockObserver : public IObserver {
public:
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, onGameEvent, (GameEventType eventType, const std::string& teamName), (override));
};

// Test fixture for Subject tests
class SubjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        subject = std::make_unique<Subject>();
        observer1 = std::make_shared<MockObserver>();
        observer2 = std::make_shared<MockObserver>();
        observer3 = std::make_shared<MockObserver>();
    }

    void TearDown() override {
        subject.reset();
        observer1.reset();
        observer2.reset();
        observer3.reset();
    }

    std::unique_ptr<Subject> subject;
    std::shared_ptr<MockObserver> observer1;
    std::shared_ptr<MockObserver> observer2;
    std::shared_ptr<MockObserver> observer3;
};

// Test adding a single observer
TEST_F(SubjectTest, AddSingleObserver) {
    // Add observer
    subject->addObserver(observer1);

    // Expect the observer to be called when notifying
    EXPECT_CALL(*observer1, update())
        .Times(1);

    subject->notifyObservers();
}

// Test adding multiple observers
TEST_F(SubjectTest, AddMultipleObservers) {
    // Add multiple observers
    subject->addObserver(observer1);
    subject->addObserver(observer2);
    subject->addObserver(observer3);

    // Expect all observers to be called
    EXPECT_CALL(*observer1, update())
        .Times(1);
    EXPECT_CALL(*observer2, update())
        .Times(1);
    EXPECT_CALL(*observer3, update())
        .Times(1);

    subject->notifyObservers();
}

// Test adding the same observer multiple times
TEST_F(SubjectTest, AddSameObserverMultipleTimes) {
    // Add the same observer multiple times
    subject->addObserver(observer1);
    subject->addObserver(observer1);
    subject->addObserver(observer1);

    // Observer should be called multiple times (once for each addition)
    EXPECT_CALL(*observer1, update())
        .Times(3);

    subject->notifyObservers();
}

// Test adding null observer (should not crash)
TEST_F(SubjectTest, AddNullObserver) {
    std::shared_ptr<IObserver> nullObserver = nullptr;

    // Adding null observer should not crash
    EXPECT_NO_THROW(subject->addObserver(nullObserver));

    // Notifying should not crash either
    EXPECT_NO_THROW(subject->notifyObservers());
}

// Test removing an observer
TEST_F(SubjectTest, RemoveObserver) {
    // Add observers
    subject->addObserver(observer1);
    subject->addObserver(observer2);

    // Remove one observer
    subject->removeObserver(observer1);

    // Only observer2 should be called
    EXPECT_CALL(*observer1, update())
        .Times(0);
    EXPECT_CALL(*observer2, update())
        .Times(1);

    subject->notifyObservers();
}

// Test removing an observer that was never added
TEST_F(SubjectTest, RemoveNonExistentObserver) {
    // Add only observer1
    subject->addObserver(observer1);

    // Try to remove observer2 (not added)
    EXPECT_NO_THROW(subject->removeObserver(observer2));

    // observer1 should still be called
    EXPECT_CALL(*observer1, update())
        .Times(1);

    subject->notifyObservers();
}

// Test removing all observers
TEST_F(SubjectTest, RemoveAllObservers) {
    // Add observers
    subject->addObserver(observer1);
    subject->addObserver(observer2);
    subject->addObserver(observer3);

    // Remove all observers
    subject->removeObserver(observer1);
    subject->removeObserver(observer2);
    subject->removeObserver(observer3);

    // No observers should be called
    EXPECT_CALL(*observer1, update())
        .Times(0);
    EXPECT_CALL(*observer2, update())
        .Times(0);
    EXPECT_CALL(*observer3, update())
        .Times(0);

    subject->notifyObservers();
}

// Test removing null observer (should not crash)
TEST_F(SubjectTest, RemoveNullObserver) {
    std::shared_ptr<IObserver> nullObserver = nullptr;

    // Add a valid observer
    subject->addObserver(observer1);

    // Removing null observer should not crash
    EXPECT_NO_THROW(subject->removeObserver(nullObserver));

    // Valid observer should still work
    EXPECT_CALL(*observer1, update())
        .Times(1);

    subject->notifyObservers();
}

// Test notifying observers when no observers are added
TEST_F(SubjectTest, NotifyWithoutObservers) {
    // Should not crash when no observers are present
    EXPECT_NO_THROW(subject->notifyObservers());
}

// Test that expired weak_ptr observers are automatically cleaned up
TEST_F(SubjectTest, ExpiredObserversCleanup) {
    // Add observer
    subject->addObserver(observer1);

    // Verify observer is called initially
    EXPECT_CALL(*observer1, update())
        .Times(1);
    subject->notifyObservers();

    // Reset the shared_ptr to simulate observer destruction
    observer1.reset();

    // Notifying should clean up expired weak_ptr automatically
    EXPECT_NO_THROW(subject->notifyObservers());
}

// Test notifyGameEvent with STATE_CHANGED event
TEST_F(SubjectTest, NotifyGameEventStateChanged) {
    subject->addObserver(observer1);
    subject->addObserver(observer2);

    std::string teamName = "TestTeam";

    EXPECT_CALL(*observer1, onGameEvent(GameEventType::STATE_CHANGED, teamName))
        .Times(1);
    EXPECT_CALL(*observer2, onGameEvent(GameEventType::STATE_CHANGED, teamName))
        .Times(1);

    subject->notifyGameEvent(GameEventType::STATE_CHANGED, teamName);
}

// Test notifyGameEvent with TEAM_WIN event
TEST_F(SubjectTest, NotifyGameEventTeamWin) {
    subject->addObserver(observer1);

    std::string teamName = "WinningTeam";

    EXPECT_CALL(*observer1, onGameEvent(GameEventType::TEAM_WIN, teamName))
        .Times(1);

    subject->notifyGameEvent(GameEventType::TEAM_WIN, teamName);
}

// Test notifyGameEvent with TEAM_DEFEAT event
TEST_F(SubjectTest, NotifyGameEventTeamDefeat) {
    subject->addObserver(observer1);

    std::string teamName = "LosingTeam";

    EXPECT_CALL(*observer1, onGameEvent(GameEventType::TEAM_DEFEAT, teamName))
        .Times(1);

    subject->notifyGameEvent(GameEventType::TEAM_DEFEAT, teamName);
}

// Test notifyGameEvent with empty team name
TEST_F(SubjectTest, NotifyGameEventEmptyTeamName) {
    subject->addObserver(observer1);

    std::string emptyTeamName = "";

    EXPECT_CALL(*observer1, onGameEvent(GameEventType::TEAM_WIN, emptyTeamName))
        .Times(1);

    subject->notifyGameEvent(GameEventType::TEAM_WIN, emptyTeamName);
}

// Test notifyGameEvent without observers
TEST_F(SubjectTest, NotifyGameEventWithoutObservers) {
    std::string teamName = "TestTeam";

    // Should not crash when no observers are present
    EXPECT_NO_THROW(subject->notifyGameEvent(GameEventType::STATE_CHANGED, teamName));
}

// Test notifyGameEvent with expired observers
TEST_F(SubjectTest, NotifyGameEventExpiredObservers) {
    subject->addObserver(observer1);

    // Reset the shared_ptr to simulate observer destruction
    observer1.reset();

    std::string teamName = "TestTeam";

    // Should clean up expired observers and not crash
    EXPECT_NO_THROW(subject->notifyGameEvent(GameEventType::TEAM_WIN, teamName));
}

// Test mixed scenarios: add, remove, notify
TEST_F(SubjectTest, MixedOperations) {
    // Add observers
    subject->addObserver(observer1);
    subject->addObserver(observer2);
    subject->addObserver(observer3);

    // First notification - all should be called
    EXPECT_CALL(*observer1, update()).Times(1);
    EXPECT_CALL(*observer2, update()).Times(1);
    EXPECT_CALL(*observer3, update()).Times(1);
    subject->notifyObservers();

    // Remove middle observer
    subject->removeObserver(observer2);

    // Second notification - only observer1 and observer3
    EXPECT_CALL(*observer1, update()).Times(1);
    EXPECT_CALL(*observer2, update()).Times(0);
    EXPECT_CALL(*observer3, update()).Times(1);
    subject->notifyObservers();

    // Game event notification
    std::string teamName = "TestTeam";
    EXPECT_CALL(*observer1, onGameEvent(GameEventType::TEAM_WIN, teamName)).Times(1);
    EXPECT_CALL(*observer2, onGameEvent(GameEventType::TEAM_WIN, teamName)).Times(0);
    EXPECT_CALL(*observer3, onGameEvent(GameEventType::TEAM_WIN, teamName)).Times(1);
    subject->notifyGameEvent(GameEventType::TEAM_WIN, teamName);
}

// Test that observer methods are called with correct parameters
TEST_F(SubjectTest, CorrectParameterPassing) {
    subject->addObserver(observer1);

    std::string expectedTeamName = "SpecificTeamName";
    GameEventType expectedEventType = GameEventType::TEAM_DEFEAT;

    EXPECT_CALL(*observer1, onGameEvent(expectedEventType, expectedTeamName))
        .Times(1);

    subject->notifyGameEvent(expectedEventType, expectedTeamName);
}
