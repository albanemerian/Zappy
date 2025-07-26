/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos_test
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../gui/src/Game/GameInfos.hpp"
#include "../../../../gui/src/Exceptions/Exceptions.hpp"

// Mock for ICommunication
class MockCommunication : public ICommunication {
public:
    MOCK_METHOD(void, sendMessage, (const std::string &message), (override));
    MOCK_METHOD(bool, hasMessages, (), (const, override));
    MOCK_METHOD(std::string, popMessage, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
    MOCK_METHOD(void, disconnect, (), (override));
};

// Mock for IAudio
class MockAudio : public IAudio {
public:
    MOCK_METHOD(float, getSFXVolumeLevel, (), (override));
    MOCK_METHOD(float, getMusicVolumeLevel, (), (override));
    MOCK_METHOD(void, setSFXVolumeLevel, (float), (override));
    MOCK_METHOD(void, setMusicVolumeLevel, (float), (override));
    MOCK_METHOD(bool, loadSound, (const std::string& id, const std::string& filepath), (override));
    MOCK_METHOD(void, playMainTheme, (float volume), (override));
    MOCK_METHOD(void, playNextTheme, (float volume), (override));
    MOCK_METHOD(void, playSound, (const std::string& id, float volume), (override));
    MOCK_METHOD(void, stopSound, (const std::string& id), (override));
    MOCK_METHOD(bool, isSoundPlaying, (const std::string& id), (const, override));
    MOCK_METHOD(void, setSoundLooping, (const std::string& id, bool looping), (override));
    MOCK_METHOD(void, setSoundVolume, (const std::string& id, float volume), (override));
};

class GameInfosTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCommunication = std::make_shared<testing::NiceMock<MockCommunication>>();
        mockAudio = std::make_shared<testing::NiceMock<MockAudio>>();
        gameInfos = std::make_unique<GameInfos>(mockCommunication);
        gameInfos->setAudio(mockAudio);
    }

    void TearDown() override {
        gameInfos.reset();
        mockCommunication.reset();
        mockAudio.reset();
    }

    std::unique_ptr<GameInfos> gameInfos;
    std::shared_ptr<testing::NiceMock<MockCommunication>> mockCommunication;
    std::shared_ptr<testing::NiceMock<MockAudio>> mockAudio;
};

// Special test observer that exposes when update is called
class TestObserver : public IObserver {
public:
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, onGameEvent, (GameEventType, const std::string&), (override));

    class TestableGameInfos : public GameInfos {
    public:
        TestableGameInfos(std::shared_ptr<ICommunication> communication)
            : GameInfos(communication) {}

        void testNotifyObservers() {
            notifyObservers();
        }
    };
};

class GameInfosObserverTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCommunication = std::make_shared<testing::NiceMock<MockCommunication>>();
        mockAudio = std::make_shared<testing::NiceMock<MockAudio>>();
        mockObserver = std::make_shared<TestObserver>();
        gameInfos = std::make_unique<TestObserver::TestableGameInfos>(mockCommunication);
        gameInfos->setAudio(mockAudio);
        gameInfos->addObserver(mockObserver);
    }

    void TearDown() override {
        gameInfos.reset();
        mockCommunication.reset();
        mockAudio.reset();
        mockObserver.reset();
    }

    std::unique_ptr<TestObserver::TestableGameInfos> gameInfos;
    std::shared_ptr<testing::NiceMock<MockCommunication>> mockCommunication;
    std::shared_ptr<testing::NiceMock<MockAudio>> mockAudio;
    std::shared_ptr<TestObserver> mockObserver;
};

// Mock Observer to test notifications
class MockObserver : public IObserver {
public:
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, onGameEvent, (GameEventType, const std::string&), (override));
};

class GameInfosAdditionalTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCommunication = std::make_shared<testing::NiceMock<MockCommunication>>();
        mockAudio = std::make_shared<testing::NiceMock<MockAudio>>();
        mockObserver = std::make_shared<testing::NiceMock<MockObserver>>();
        gameInfos = std::make_unique<GameInfos>(mockCommunication);
        gameInfos->setAudio(mockAudio);
        gameInfos->addObserver(mockObserver);
    }

    void TearDown() override {
        gameInfos.reset();
        mockCommunication.reset();
        mockAudio.reset();
        mockObserver.reset();
    }

    std::unique_ptr<GameInfos> gameInfos;
    std::shared_ptr<testing::NiceMock<MockCommunication>> mockCommunication;
    std::shared_ptr<testing::NiceMock<MockAudio>> mockAudio;
    std::shared_ptr<testing::NiceMock<MockObserver>> mockObserver;
};

// Map size tests
TEST_F(GameInfosTest, SetAndGetMapSize) {
    gameInfos->setMapSize(10, 20);
    auto size = gameInfos->getMapSize();

    EXPECT_EQ(size.first, 10);
    EXPECT_EQ(size.second, 20);
}

// Time unit tests
TEST_F(GameInfosTest, SetAndGetTimeUnit) {
    gameInfos->setTimeUnit(100);

    EXPECT_EQ(gameInfos->getTimeUnit(), 100);
}

// Tile tests
TEST_F(GameInfosTest, UpdateAndGetTile) {
    zappy::structs::Tile tile(5, 8, 10, 1, 2, 3, 4, 5, 6);
    gameInfos->setMapSize(20, 20);
    gameInfos->updateTile(tile);

    auto retrievedTile = gameInfos->getTile(5, 8);

    EXPECT_EQ(retrievedTile.x, 5);
    EXPECT_EQ(retrievedTile.y, 8);
    EXPECT_EQ(retrievedTile.food, 10);
    EXPECT_EQ(retrievedTile.linemate, 1);
    EXPECT_EQ(retrievedTile.deraumere, 2);
    EXPECT_EQ(retrievedTile.sibur, 3);
    EXPECT_EQ(retrievedTile.mendiane, 4);
    EXPECT_EQ(retrievedTile.phiras, 5);
    EXPECT_EQ(retrievedTile.thystame, 6);
}

TEST_F(GameInfosTest, GetNonexistentTile) {
    gameInfos->setMapSize(20, 20);

    auto retrievedTile = gameInfos->getTile(15, 15);

    EXPECT_EQ(retrievedTile.x, 15);
    EXPECT_EQ(retrievedTile.y, 15);
    EXPECT_EQ(retrievedTile.food, 0);
    EXPECT_EQ(retrievedTile.linemate, 0);
    EXPECT_EQ(retrievedTile.deraumere, 0);
    EXPECT_EQ(retrievedTile.sibur, 0);
    EXPECT_EQ(retrievedTile.mendiane, 0);
    EXPECT_EQ(retrievedTile.phiras, 0);
    EXPECT_EQ(retrievedTile.thystame, 0);
}

TEST_F(GameInfosTest, UpdateExistingTile) {
    gameInfos->setMapSize(20, 20);

    zappy::structs::Tile tile1(5, 8, 10, 1, 2, 3, 4, 5, 6);
    gameInfos->updateTile(tile1);

    zappy::structs::Tile tile2(5, 8, 20, 2, 3, 4, 5, 6, 7);
    gameInfos->updateTile(tile2);

    auto retrievedTile = gameInfos->getTile(5, 8);

    EXPECT_EQ(retrievedTile.food, 20);
    EXPECT_EQ(retrievedTile.linemate, 2);
}

TEST_F(GameInfosTest, UpdateTileOutOfBounds) {
    gameInfos->setMapSize(10, 10);

    zappy::structs::Tile tile(15, 15, 10, 1, 2, 3, 4, 5, 6);
    gameInfos->updateTile(tile);

    auto tiles = gameInfos->getTileRef(15, 15);
    EXPECT_EQ(tiles.x, 0);
}

TEST_F(GameInfosTest, GetAllTiles) {
    gameInfos->setMapSize(10, 10);

    zappy::structs::Tile tile1(1, 1, 10, 1, 2, 3, 4, 5, 6);
    zappy::structs::Tile tile2(2, 2, 20, 2, 3, 4, 5, 6, 7);

    gameInfos->updateTile(tile1);
    gameInfos->updateTile(tile2);

    auto tile_1 = gameInfos->getTileRef(1, 1);
    auto tile_2 = gameInfos->getTileRef(2, 2);

    EXPECT_EQ(tile_1.x, 1);
    EXPECT_EQ(tile_1.y, 1);
    EXPECT_EQ(tile_2.x, 2);
    EXPECT_EQ(tile_2.y, 2);
}

// Team name tests
TEST_F(GameInfosTest, UpdateAndGetTeamNames) {
    gameInfos->updateTeamName("Team1");
    gameInfos->updateTeamName("Team2");

    auto teamNames = gameInfos->getTeamNames();

    EXPECT_EQ(teamNames.size(), 2);
    EXPECT_EQ(teamNames[0], "Team1");
    EXPECT_EQ(teamNames[1], "Team2");
}

TEST_F(GameInfosTest, DuplicateTeamName) {
    gameInfos->updateTeamName("Team1");
    gameInfos->updateTeamName("Team1");

    auto teamNames = gameInfos->getTeamNames();

    EXPECT_EQ(teamNames.size(), 1);
    EXPECT_EQ(teamNames[0], "Team1");
}

// Player tests
TEST_F(GameInfosTest, AddAndGetPlayer) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(players[0].number, 1);
    EXPECT_EQ(players[0].x, 5);
    EXPECT_EQ(players[0].y, 5);
    EXPECT_EQ(players[0].orientation, 1);
    EXPECT_EQ(players[0].level, 3);
    EXPECT_EQ(players[0].teamName, "Team1");
}

TEST_F(GameInfosTest, UpdateExistingPlayer) {
    zappy::structs::Player player1(1, 5, 5, 1, 3, "Team1");
    zappy::structs::Player player2(1, 10, 10, 2, 4, "Team1");

    gameInfos->addPlayer(player1);
    gameInfos->addPlayer(player2);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(players[0].number, 1);
    EXPECT_EQ(players[0].x, 10);
    EXPECT_EQ(players[0].y, 10);
    EXPECT_EQ(players[0].orientation, 2);
    EXPECT_EQ(players[0].level, 4);
}

TEST_F(GameInfosTest, UpdatePlayerPosition) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    gameInfos->setMapSize(20, 20);

    gameInfos->updatePlayerPosition(1, 10, 15);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players[0].x, 10);
    EXPECT_EQ(players[0].y, 15);
}

TEST_F(GameInfosTest, UpdatePlayerPositionOutOfBounds) {
    gameInfos->setMapSize(10, 10);
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerPosition(1, 20, 20);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players[0].x, 5);
    EXPECT_EQ(players[0].y, 5);
}

TEST_F(GameInfosTest, UpdatePlayerLevel) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerLevel(1, 5);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players[0].level, 5);
}

TEST_F(GameInfosTest, UpdatePlayerInventory) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    zappy::structs::Inventory inventory(10, 1, 2, 3, 4, 5, 6);
    gameInfos->updatePlayerInventory(1, inventory);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players[0].inventory.food, 10);
    EXPECT_EQ(players[0].inventory.linemate, 1);
    EXPECT_EQ(players[0].inventory.deraumere, 2);
    EXPECT_EQ(players[0].inventory.sibur, 3);
    EXPECT_EQ(players[0].inventory.mendiane, 4);
    EXPECT_EQ(players[0].inventory.phiras, 5);
    EXPECT_EQ(players[0].inventory.thystame, 6);
}

TEST_F(GameInfosTest, PlayerDeath) {
    zappy::structs::Player player1(1, 5, 5, 1, 3, "Team1");
    zappy::structs::Player player2(2, 6, 6, 1, 3, "Team1");

    gameInfos->addPlayer(player1);
    gameInfos->addPlayer(player2);

    gameInfos->updatePlayerDeath(1);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(players[0].number, 2);
    gameInfos->updatePlayerDeath(-1);
}

// Broadcast tests
TEST_F(GameInfosTest, PlayerBroadcast) {
    gameInfos->addPlayerBroadcast(1, "Hello World");

    auto broadcasts = gameInfos->getPlayersBroadcasting();

    EXPECT_EQ(broadcasts.size(), 1);
    EXPECT_EQ(broadcasts[0].first, 1);
    EXPECT_EQ(broadcasts[0].second, "Hello World");
}

TEST_F(GameInfosTest, EmptyBroadcast) {
    gameInfos->addPlayerBroadcast(1, "");

    auto broadcasts = gameInfos->getPlayersBroadcasting();

    EXPECT_EQ(broadcasts.size(), 0);
}

// Incantation tests
TEST_F(GameInfosTest, AddAndRemoveIncantation) {
    std::vector<int> players = {1, 2, 3};
    zappy::structs::Incantation incantation(5, 5, 2, players);

    gameInfos->addIncantation(incantation);
    gameInfos->removeIncantation(5, 5, 1);

    SUCCEED();
}

// Egg tests
TEST_F(GameInfosTest, AddAndGetEgg) {
    zappy::structs::Egg egg(1, 2, 5, 5, false, "Team1");
    gameInfos->addEgg(egg);
    gameInfos->addEgg(egg);

    auto eggs = gameInfos->getEggs();

    EXPECT_EQ(eggs.size(), 1);
    EXPECT_EQ(eggs[0].eggNumber, 1);
    EXPECT_EQ(eggs[0].playerNumber, 2);
    EXPECT_EQ(eggs[0].x, 5);
    EXPECT_EQ(eggs[0].y, 5);
    EXPECT_FALSE(eggs[0].hatched);
    EXPECT_EQ(eggs[0].teamName, "Team1");
}

TEST_F(GameInfosTest, UpdateEggHatched) {
    zappy::structs::Egg egg(1, 2, 5, 5, false, "Team1");
    gameInfos->addEgg(egg);

    gameInfos->updateEggHatched(1);

    auto eggs = gameInfos->getEggs();

    EXPECT_TRUE(eggs[0].hatched);
}

TEST_F(GameInfosTest, EggDeath) {
    zappy::structs::Egg egg1(1, 2, 5, 5, false, "Team1");
    zappy::structs::Egg egg2(2, 3, 6, 6, false, "Team1");

    gameInfos->addEgg(egg1);
    gameInfos->addEgg(egg2);

    gameInfos->updateEggDeath(1);

    auto eggs = gameInfos->getEggs();

    EXPECT_EQ(eggs.size(), 1);
    EXPECT_EQ(eggs[0].eggNumber, 2);
}

// Game over tests
TEST_F(GameInfosTest, SetAndCheckGameOver) {
    gameInfos->setGameOver("Team1");

    auto gameOverInfo = gameInfos->isGameOver();

    EXPECT_TRUE(gameOverInfo.first);
    EXPECT_EQ(gameOverInfo.second, "Team1");
}

TEST_F(GameInfosTest, InitialGameOverStatus) {
    auto gameOverInfo = gameInfos->isGameOver();

    EXPECT_FALSE(gameOverInfo.first);
    EXPECT_TRUE(gameOverInfo.second.empty());
}

// updatePlayerExpulsion tests
TEST_F(GameInfosTest, UpdatePlayerExpulsion) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerExpulsion(1);
    gameInfos->updatePlayerExpulsion(1);
    gameInfos->updatePlayerExpulsion(2);
    gameInfos->updatePlayerExpulsion(-1);
}

// updatePlayerResourceAction tests
TEST_F(GameInfosTest, UpdatePlayerResourceAction) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerResourceAction(1, 0, true);
    gameInfos->updatePlayerResourceAction(1, 1, false);

    SUCCEED();
}

// updatePlayerFork tests
TEST_F(GameInfosTest, UpdatePlayerFork) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerFork(1);
    SUCCEED();
}

// Test audio integration
TEST_F(GameInfosTest, PlayDefeatSound) {
    EXPECT_CALL(*mockAudio, playSound(testing::_, testing::_)).Times(1);

    gameInfos->playDefeatSound("Team1");
}

// Test the notifyObservers method which is called by notifyStateChange
TEST_F(GameInfosObserverTest, NotifyObservers) {
    EXPECT_CALL(*mockObserver, update()).Times(1);

    gameInfos->testNotifyObservers();
}

// Test killPlayer method
TEST_F(GameInfosAdditionalTest, KillPlayer) {
    gameInfos->killPlayer(-1);

    EXPECT_CALL(*mockCommunication, sendMessage("kil #5\n")).Times(1);
    gameInfos->killPlayer(5);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());
    EXPECT_CALL(*mockCommunication, sendMessage("kil #10\n")).Times(1);
    gameInfos->killPlayer(10);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());
    ON_CALL(*mockCommunication, sendMessage).WillByDefault(
        [](const std::string &) { throw Exceptions::NetworkException("Test exception"); }
    );
    gameInfos->killPlayer(15);
}

// Test getIncantations method
TEST_F(GameInfosAdditionalTest, GetIncantations) {
    std::vector<int> players = {1, 2, 3};
    zappy::structs::Incantation incantation1(5, 5, 2, players);
    zappy::structs::Incantation incantation2(10, 10, 3, players);

    gameInfos->addPlayer(zappy::structs::Player(1, 5, 5, 1, 3, "Team1"));
    gameInfos->addPlayer(zappy::structs::Player(2, 10, 10, 2, 4, "Team1"));
    gameInfos->addPlayer(zappy::structs::Player(3, 15, 15, 3, 5, "Team1"));
    gameInfos->addIncantation(incantation1);
    gameInfos->addIncantation(incantation2);

    auto incantations = gameInfos->getIncantations();

    EXPECT_EQ(incantations.size(), 2);
    EXPECT_EQ(incantations[0].x, 5);
    EXPECT_EQ(incantations[0].y, 5);
    EXPECT_EQ(incantations[0].level, 2);
    EXPECT_EQ(incantations[1].x, 10);
    EXPECT_EQ(incantations[1].y, 10);
    EXPECT_EQ(incantations[1].level, 3);
}

// Test getPlayer method
TEST_F(GameInfosAdditionalTest, GetPlayer) {
    zappy::structs::Player player1(1, 5, 5, 1, 3, "Team1");
    zappy::structs::Player player2(2, 10, 10, 2, 4, "Team2");

    gameInfos->addPlayer(player1);
    gameInfos->addPlayer(player2);

    auto retrievedPlayer = gameInfos->getPlayer(2);

    EXPECT_EQ(retrievedPlayer.number, 2);
    EXPECT_EQ(retrievedPlayer.x, 10);
    EXPECT_EQ(retrievedPlayer.y, 10);
    EXPECT_EQ(retrievedPlayer.orientation, 2);
    EXPECT_EQ(retrievedPlayer.level, 4);
    EXPECT_EQ(retrievedPlayer.teamName, "Team2");

    auto nonExistentPlayer = gameInfos->getPlayer(99);
    EXPECT_EQ(nonExistentPlayer.number, 0);
}

// Test observer notification - we can't directly call notifyStateChange as it's private
TEST_F(GameInfosAdditionalTest, NotifyObserver) {
    EXPECT_CALL(*mockObserver, onGameEvent(GameEventType::TEAM_WIN, "TestTeam")).Times(1);
    gameInfos->setGameOver("TestTeam");
}

// Test updatePlayerOrientation with invalid orientation
TEST_F(GameInfosAdditionalTest, UpdatePlayerOrientationInvalid) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerOrientation(1, 0);
    gameInfos->updatePlayerOrientation(1, 5);

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players[0].orientation, 1);
}

// Additional tests for exception handling paths
TEST_F(GameInfosAdditionalTest, SetGameOverExceptionHandling) {
    ON_CALL(*mockAudio, playSound).WillByDefault(
        [](const std::string &, float) { throw std::runtime_error("Test exception"); }
    );

    gameInfos->setGameOver("Team1");

    auto gameOverInfo = gameInfos->isGameOver();
    EXPECT_TRUE(gameOverInfo.first);
    EXPECT_EQ(gameOverInfo.second, "Team1");
}

TEST_F(GameInfosAdditionalTest, PlayDefeatSoundExceptionHandling) {
    ON_CALL(*mockAudio, playSound).WillByDefault(
        [](const std::string &, float) { throw std::runtime_error("Test exception"); }
    );

    EXPECT_NO_THROW(gameInfos->playDefeatSound("Team1"));
}

// Test updatePlayerResourceAction with invalid resource ID
TEST_F(GameInfosAdditionalTest, UpdatePlayerResourceActionInvalidResource) {
    gameInfos->updatePlayerResourceAction(1, -1, true);
    gameInfos->updatePlayerResourceAction(1, 7, true);

    SUCCEED();
}

// Test updatePlayerDeath with non-existent player
TEST_F(GameInfosAdditionalTest, UpdatePlayerDeathNonExistentPlayer) {
    gameInfos->updatePlayerDeath(99);

    SUCCEED();
}

// Test setTimeUnit with server communication
TEST_F(GameInfosAdditionalTest, SetTimeUnitWithServerCommunication) {
    EXPECT_CALL(*mockCommunication, sendMessage("sst 200\n")).Times(1);
    gameInfos->setTimeUnit(200, true);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());
    EXPECT_CALL(*mockCommunication, sendMessage("sst 300\n")).Times(1);
    gameInfos->setTimeUnit(300, true);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());
    ON_CALL(*mockCommunication, sendMessage).WillByDefault(
        [](const std::string &) { throw Exceptions::NetworkException("Test exception"); }
    );
    gameInfos->setTimeUnit(400, true);
}

// Tests for new GameInfos methods

// Test incrementPlayerLevel method
TEST_F(GameInfosAdditionalTest, IncrementPlayerLevel) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    EXPECT_CALL(*mockCommunication, sendMessage("plu #1\n")).Times(1);
    gameInfos->incrementPlayerLevel(1);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    gameInfos->updatePlayerLevel(1, 8);
    gameInfos->incrementPlayerLevel(1);

    gameInfos->incrementPlayerLevel(999);

    EXPECT_CALL(*mockCommunication, sendMessage("plu #1\n")).WillOnce(
        testing::Throw(Exceptions::NetworkException("Network error"))
    );
    gameInfos->updatePlayerLevel(1, 5);
    gameInfos->incrementPlayerLevel(1);
}

// Test decrementPlayerLevel method
TEST_F(GameInfosAdditionalTest, DecrementPlayerLevel) {
    zappy::structs::Player player(1, 5, 5, 1, 5, "Team1");
    gameInfos->addPlayer(player);

    EXPECT_CALL(*mockCommunication, sendMessage("pld #1\n")).Times(1);
    gameInfos->decrementPlayerLevel(1);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    gameInfos->updatePlayerLevel(1, 1);
    gameInfos->decrementPlayerLevel(1);

    gameInfos->decrementPlayerLevel(999);

    EXPECT_CALL(*mockCommunication, sendMessage("pld #1\n")).WillOnce(
        testing::Throw(Exceptions::NetworkException("Network error"))
    );
    gameInfos->updatePlayerLevel(1, 3);
    gameInfos->decrementPlayerLevel(1);
}

// Test securityActualisation method
TEST_F(GameInfosAdditionalTest, SecurityActualisation) {
    zappy::structs::Player player1(1, 5, 5, 1, 3, "Team1");
    zappy::structs::Player player2(2, 10, 10, 2, 4, "Team2");
    gameInfos->addPlayer(player1);
    gameInfos->addPlayer(player2);

    EXPECT_CALL(*mockCommunication, sendMessage("msz\n")).Times(1);
    EXPECT_CALL(*mockCommunication, sendMessage("tna\n")).Times(1);
    EXPECT_CALL(*mockCommunication, sendMessage("sgt\n")).Times(1);
    EXPECT_CALL(*mockCommunication, sendMessage("mct\n")).Times(1);

    EXPECT_CALL(*mockCommunication, sendMessage("ppo #1\n")).Times(1);
    EXPECT_CALL(*mockCommunication, sendMessage("plv #1\n")).Times(1);
    EXPECT_CALL(*mockCommunication, sendMessage("pin #1\n")).Times(1);
    EXPECT_CALL(*mockCommunication, sendMessage("ppo #2\n")).Times(1);
    EXPECT_CALL(*mockCommunication, sendMessage("plv #2\n")).Times(1);
    EXPECT_CALL(*mockCommunication, sendMessage("pin #2\n")).Times(1);

    gameInfos->securityActualisation();
}

// Test securityActualisation with network exception
TEST_F(GameInfosAdditionalTest, SecurityActualisationNetworkException) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    ON_CALL(*mockCommunication, sendMessage).WillByDefault(
        [](const std::string &) { throw Exceptions::NetworkException("Network error"); }
    );

    EXPECT_NO_THROW(gameInfos->securityActualisation());
}

// Test getServerMessages and addServerMessage methods
TEST_F(GameInfosAdditionalTest, ServerMessages) {
    auto messages = gameInfos->getServerMessages();
    EXPECT_TRUE(messages.empty());

    gameInfos->addServerMessage("First message");
    gameInfos->addServerMessage("Second message");
    gameInfos->addServerMessage("Third message");

    messages = gameInfos->getServerMessages();
    EXPECT_EQ(messages.size(), 3);
    EXPECT_EQ(messages[0], "First message");
    EXPECT_EQ(messages[1], "Second message");
    EXPECT_EQ(messages[2], "Third message");

    gameInfos->addServerMessage("");
    messages = gameInfos->getServerMessages();
    EXPECT_EQ(messages.size(), 4);
    EXPECT_EQ(messages[3], "");
}

// Test team visibility methods
TEST_F(GameInfosAdditionalTest, TeamVisibility) {
    gameInfos->setTeamVisibility("Team1", false);
    gameInfos->setTeamVisibility("Team2", true);
    gameInfos->setTeamVisibility("Team3", false);

    EXPECT_FALSE(gameInfos->isTeamVisible("Team1"));
    EXPECT_TRUE(gameInfos->isTeamVisible("Team2"));
    EXPECT_FALSE(gameInfos->isTeamVisible("Team3"));

    EXPECT_TRUE(gameInfos->isTeamVisible("NonExistentTeam"));

    auto visibilities = gameInfos->getTeamVisibilities();
    EXPECT_EQ(visibilities.size(), 3);
    EXPECT_FALSE(visibilities["Team1"]);
    EXPECT_TRUE(visibilities["Team2"]);
    EXPECT_FALSE(visibilities["Team3"]);

    gameInfos->setTeamVisibility("Team1", true);
    EXPECT_TRUE(gameInfos->isTeamVisible("Team1"));
}

// Test incrementPlayerInventoryItem method
TEST_F(GameInfosAdditionalTest, IncrementPlayerInventoryItem) {
    EXPECT_CALL(*mockCommunication, sendMessage("pia #1 2\n")).Times(1);
    gameInfos->incrementPlayerInventoryItem(1, 2);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    for (int resourceId = 0; resourceId <= 6; ++resourceId) {
        EXPECT_CALL(*mockCommunication, sendMessage("pia #5 " + std::to_string(resourceId) + "\n")).Times(1);
        gameInfos->incrementPlayerInventoryItem(5, resourceId);
    }

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    gameInfos->incrementPlayerInventoryItem(-1, 2);
    gameInfos->incrementPlayerInventoryItem(1, -1);
    gameInfos->incrementPlayerInventoryItem(1, 7);

    EXPECT_CALL(*mockCommunication, sendMessage("pia #1 3\n")).WillOnce(
        testing::Throw(Exceptions::NetworkException("Network error"))
    );
    EXPECT_NO_THROW(gameInfos->incrementPlayerInventoryItem(1, 3));
}

// Test decrementPlayerInventoryItem method
TEST_F(GameInfosAdditionalTest, DecrementPlayerInventoryItem) {
    EXPECT_CALL(*mockCommunication, sendMessage("pis #1 2\n")).Times(1);
    gameInfos->decrementPlayerInventoryItem(1, 2);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    for (int resourceId = 0; resourceId <= 6; ++resourceId) {
        EXPECT_CALL(*mockCommunication, sendMessage("pis #5 " + std::to_string(resourceId) + "\n")).Times(1);
        gameInfos->decrementPlayerInventoryItem(5, resourceId);
    }

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    gameInfos->decrementPlayerInventoryItem(-1, 2);
    gameInfos->decrementPlayerInventoryItem(1, -1);
    gameInfos->decrementPlayerInventoryItem(1, 7);

    EXPECT_CALL(*mockCommunication, sendMessage("pis #1 3\n")).WillOnce(
        testing::Throw(Exceptions::NetworkException("Network error"))
    );
    EXPECT_NO_THROW(gameInfos->decrementPlayerInventoryItem(1, 3));
}

// Test incrementTileInventoryItem method
TEST_F(GameInfosAdditionalTest, IncrementTileInventoryItem) {
    EXPECT_CALL(*mockCommunication, sendMessage("tar 5 10 2\n")).Times(1);
    gameInfos->incrementTileInventoryItem(5, 10, 2);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    for (int resourceId = 0; resourceId <= 6; ++resourceId) {
        EXPECT_CALL(*mockCommunication, sendMessage("tar 3 7 " + std::to_string(resourceId) + "\n")).Times(1);
        gameInfos->incrementTileInventoryItem(3, 7, resourceId);
    }

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    gameInfos->incrementTileInventoryItem(-1, 5, 2);
    gameInfos->incrementTileInventoryItem(5, -1, 2);
    gameInfos->incrementTileInventoryItem(5, 10, -1);
    gameInfos->incrementTileInventoryItem(5, 10, 7);

    EXPECT_CALL(*mockCommunication, sendMessage("tar 5 10 3\n")).WillOnce(
        testing::Throw(Exceptions::NetworkException("Network error"))
    );
    EXPECT_NO_THROW(gameInfos->incrementTileInventoryItem(5, 10, 3));
}

// Test decrementTileInventoryItem method
TEST_F(GameInfosAdditionalTest, DecrementTileInventoryItem) {
    EXPECT_CALL(*mockCommunication, sendMessage("tsr 5 10 2\n")).Times(1);
    gameInfos->decrementTileInventoryItem(5, 10, 2);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    for (int resourceId = 0; resourceId <= 6; ++resourceId) {
        EXPECT_CALL(*mockCommunication, sendMessage("tsr 3 7 " + std::to_string(resourceId) + "\n")).Times(1);
        gameInfos->decrementTileInventoryItem(3, 7, resourceId);
    }

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());

    gameInfos->decrementTileInventoryItem(-1, 5, 2);
    gameInfos->decrementTileInventoryItem(5, -1, 2);
    gameInfos->decrementTileInventoryItem(5, 10, -1);
    gameInfos->decrementTileInventoryItem(5, 10, 7);

    EXPECT_CALL(*mockCommunication, sendMessage("tsr 5 10 3\n")).WillOnce(
        testing::Throw(Exceptions::NetworkException("Network error"))
    );
    EXPECT_NO_THROW(gameInfos->decrementTileInventoryItem(5, 10, 3));
}

// Test notifyStateChange (through observer)
TEST_F(GameInfosAdditionalTest, NotifyStateChangeIndirect) {
    EXPECT_CALL(*mockObserver, onGameEvent(GameEventType::TEAM_WIN, "WinningTeam")).Times(1);

    gameInfos->setGameOver("WinningTeam");

    testing::Mock::VerifyAndClearExpectations(mockObserver.get());
}

// Additional edge case tests for robustness
TEST_F(GameInfosAdditionalTest, EdgeCases) {
    zappy::structs::Player player(1, 5, 5, 1, 1, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->decrementPlayerLevel(1);

    gameInfos->updatePlayerLevel(1, 8);
    gameInfos->incrementPlayerLevel(1);

    gameInfos->setTeamVisibility("Team1", true);
    gameInfos->setTeamVisibility("Team1", true);
    EXPECT_TRUE(gameInfos->isTeamVisible("Team1"));

    gameInfos->addServerMessage("");
    auto messages = gameInfos->getServerMessages();
    EXPECT_FALSE(messages.empty());
}

// Test updateTile with uninitialized matrix
TEST_F(GameInfosAdditionalTest, UpdateTileWithUninitializedMatrix) {
    gameInfos->setMapSize(10, 10);

    zappy::structs::Tile tile(5, 5, 10, 1, 2, 3, 4, 5, 6);

    gameInfos->updateTile(tile);

    auto retrievedTile = gameInfos->getTile(5, 5);
    EXPECT_EQ(retrievedTile.x, 5);
    EXPECT_EQ(retrievedTile.y, 5);
    EXPECT_EQ(retrievedTile.food, 10);
    EXPECT_EQ(retrievedTile.linemate, 1);
    EXPECT_EQ(retrievedTile.deraumere, 2);
    EXPECT_EQ(retrievedTile.sibur, 3);
    EXPECT_EQ(retrievedTile.mendiane, 4);
    EXPECT_EQ(retrievedTile.phiras, 5);
    EXPECT_EQ(retrievedTile.thystame, 6);

    zappy::structs::Tile tile2(3, 7, 20, 2, 3, 4, 5, 6, 7);
    gameInfos->updateTile(tile2);

    auto retrievedTile2 = gameInfos->getTile(3, 7);
    EXPECT_EQ(retrievedTile2.x, 3);
    EXPECT_EQ(retrievedTile2.y, 7);
    EXPECT_EQ(retrievedTile2.food, 20);
    EXPECT_EQ(retrievedTile2.linemate, 2);
}

// Test getTile with negative coordinates
TEST_F(GameInfosAdditionalTest, GetTileWithNegativeCoordinates) {
    gameInfos->setMapSize(10, 10);

    auto tileNegativeX = gameInfos->getTile(-1, 5);
    EXPECT_EQ(tileNegativeX.x, -1);
    EXPECT_EQ(tileNegativeX.y, 5);
    EXPECT_EQ(tileNegativeX.food, 0);
    EXPECT_EQ(tileNegativeX.linemate, 0);
    EXPECT_EQ(tileNegativeX.deraumere, 0);
    EXPECT_EQ(tileNegativeX.sibur, 0);
    EXPECT_EQ(tileNegativeX.mendiane, 0);
    EXPECT_EQ(tileNegativeX.phiras, 0);
    EXPECT_EQ(tileNegativeX.thystame, 0);

    auto tileNegativeY = gameInfos->getTile(5, -1);
    EXPECT_EQ(tileNegativeY.x, 5);
    EXPECT_EQ(tileNegativeY.y, -1);
    EXPECT_EQ(tileNegativeY.food, 0);

    auto tileBothNegative = gameInfos->getTile(-5, -3);
    EXPECT_EQ(tileBothNegative.x, -5);
    EXPECT_EQ(tileBothNegative.y, -3);
    EXPECT_EQ(tileBothNegative.food, 0);

    const auto& tileRefNegative = gameInfos->getTileRef(-1, 5);
    EXPECT_EQ(tileRefNegative.x, 0);
    EXPECT_EQ(tileRefNegative.y, 0);
    EXPECT_EQ(tileRefNegative.food, 0);
}
