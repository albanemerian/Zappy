/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** MsgHandler_test - Tests for MsgHandler functionality
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include "../../../../gui/src/Client/MsgHandler.hpp"
#include "../../../../gui/src/Game/GameInfos.hpp"
#include "../../../../gui/src/Communication/ICommunication.hpp"
#include "../../../../gui/src/Exceptions/Exceptions.hpp"
#include "../../../../gui/src/Utils/Constants.hpp"

// Mock Communication class
class MockCommunication : public ICommunication {
public:
    MOCK_METHOD(void, sendMessage, (const std::string& message), (override));
    MOCK_METHOD(bool, hasMessages, (), (const, override));
    MOCK_METHOD(std::string, popMessage, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
    MOCK_METHOD(void, disconnect, (), (override));
};

class MsgHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCommunication = std::make_shared<MockCommunication>();
        gameInfos = std::make_shared<GameInfos>(mockCommunication);
    }

    void TearDown() override {
        if (msgHandler) {
            msgHandler->stop();
            msgHandler.reset();
        }
        gameInfos.reset();
        mockCommunication.reset();
    }

    std::shared_ptr<GameInfos> gameInfos;
    std::shared_ptr<MockCommunication> mockCommunication;
    std::unique_ptr<MsgHandler> msgHandler;
};

// Test Welcome Message
TEST_F(MsgHandlerTest, HandleWelcomeMessage) {
    EXPECT_CALL(*mockCommunication, sendMessage("GRAPHIC\n"))
        .Times(1);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("WELCOME");
}

// Test Map Size Message
TEST_F(MsgHandlerTest, HandleMszMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("msz 10 20");

    auto mapSize = gameInfos->getMapSize();
    EXPECT_EQ(mapSize.first, 10);
    EXPECT_EQ(mapSize.second, 20);
}

TEST_F(MsgHandlerTest, HandleMszMessageInvalid) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("msz -5 20");

    auto mapSize = gameInfos->getMapSize();
    EXPECT_EQ(mapSize.first, 0);
    EXPECT_EQ(mapSize.second, 0);
}

// Test Time Unit Messages
TEST_F(MsgHandlerTest, HandleSgtMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("sgt 100");

    EXPECT_EQ(gameInfos->getTimeUnit(), 100);
}

TEST_F(MsgHandlerTest, HandleSstMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("sst 150");

    EXPECT_EQ(gameInfos->getTimeUnit(), 150);
}

// Test Tile Content Message
TEST_F(MsgHandlerTest, HandleBctMessage) {
    gameInfos->setMapSize(20, 20);
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("bct 5 8 10 1 2 3 4 5 6");

    auto tile = gameInfos->getTile(5, 8);
    EXPECT_EQ(tile.x, 5);
    EXPECT_EQ(tile.y, 8);
    EXPECT_EQ(tile.food, 10);
    EXPECT_EQ(tile.linemate, 1);
    EXPECT_EQ(tile.deraumere, 2);
    EXPECT_EQ(tile.sibur, 3);
    EXPECT_EQ(tile.mendiane, 4);
    EXPECT_EQ(tile.phiras, 5);
    EXPECT_EQ(tile.thystame, 6);
}

// Test Team Name Message
TEST_F(MsgHandlerTest, HandleTnaMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("tna Team1");

    auto teamNames = gameInfos->getTeamNames();
    EXPECT_EQ(teamNames.size(), 1);
    EXPECT_EQ(teamNames[0], "Team1");
}

// Test Player Connection Message
TEST_F(MsgHandlerTest, HandlePnwMessage) {
    gameInfos->updateTeamName("Team1");
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pnw #1 5 10 2 3 Team1");

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(players[0].number, 1);
    EXPECT_EQ(players[0].x, 5);
    EXPECT_EQ(players[0].y, 10);
    EXPECT_EQ(players[0].orientation, 2);
    EXPECT_EQ(players[0].level, 3);
    EXPECT_EQ(players[0].teamName, "Team1");
}

TEST_F(MsgHandlerTest, HandlePnwMessageInvalidPlayerNumber) {
    gameInfos->updateTeamName("Team1");
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pnw invalid 5 10 2 3 Team1");

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players.size(), 0);
}

// Test Player Position Message
TEST_F(MsgHandlerTest, HandlePpoMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->setMapSize(20, 20);
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("ppo #1 10 15 3");

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players[0].x, 10);
    EXPECT_EQ(players[0].y, 15);
    EXPECT_EQ(players[0].orientation, 3);
}

// Test Player Level Message
TEST_F(MsgHandlerTest, HandlePlvMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("plv #1 5");

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players[0].level, 5);
}

// Test Player Inventory Message
TEST_F(MsgHandlerTest, HandlePinMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pin #1 5 10 10 1 2 3 4 5 6");

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players[0].inventory.food, 10);
    EXPECT_EQ(players[0].inventory.linemate, 1);
    EXPECT_EQ(players[0].inventory.deraumere, 2);
    EXPECT_EQ(players[0].inventory.sibur, 3);
    EXPECT_EQ(players[0].inventory.mendiane, 4);
    EXPECT_EQ(players[0].inventory.phiras, 5);
    EXPECT_EQ(players[0].inventory.thystame, 6);
}

// Test Player Expulsion Message
TEST_F(MsgHandlerTest, HandlePexMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pex #1");

    SUCCEED();
}

// Test Player Broadcast Message
TEST_F(MsgHandlerTest, HandlePbcMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pbc #1 Hello World");

    auto broadcasts = gameInfos->getPlayersBroadcasting();
    EXPECT_EQ(broadcasts.size(), 1);
    EXPECT_EQ(broadcasts[0].first, 1);
    EXPECT_EQ(broadcasts[0].second, " Hello World");
}

// Test Incantation Start Message
TEST_F(MsgHandlerTest, HandlePicMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pic 5 10 2 #1 #2 #3");

    SUCCEED();
}

// Test Incantation End Message
TEST_F(MsgHandlerTest, HandlePieMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pie 5 10 1");

    SUCCEED();
}

// Test Player Fork Message
TEST_F(MsgHandlerTest, HandlePfkMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pfk #1");

    SUCCEED();
}

// Test Player Drop Resource Message
TEST_F(MsgHandlerTest, HandlePdrMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pdr #1 0");

    SUCCEED();
}

// Test Player Collect Resource Message
TEST_F(MsgHandlerTest, HandlePgtMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pgt #1 1");

    SUCCEED();
}

// Test Player Death Message
TEST_F(MsgHandlerTest, HandlePdiMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pdi #1");

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players.size(), 0);
}

// Test Egg Laying Message
TEST_F(MsgHandlerTest, HandleEnwMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("enw #1 #2 5 10 Team1");

    SUCCEED();
}

// Test Egg Hatching Message
TEST_F(MsgHandlerTest, HandleEboMessage) {
    zappy::structs::Egg egg(1, 2, 5, 5, false, "Team1");
    gameInfos->addEgg(egg);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("ebo #1");

    auto eggs = gameInfos->getEggs();
    EXPECT_TRUE(eggs[0].hatched);
}

// Test Egg Death Message
TEST_F(MsgHandlerTest, HandleEdiMessage) {
    zappy::structs::Egg egg1(1, 2, 5, 5, false, "Team1");
    zappy::structs::Egg egg2(2, 3, 6, 6, false, "Team1");
    gameInfos->addEgg(egg1);
    gameInfos->addEgg(egg2);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("edi #1");

    auto eggs = gameInfos->getEggs();
    EXPECT_EQ(eggs.size(), 1);
    EXPECT_EQ(eggs[0].eggNumber, 2);
}

// Test Game Over Message
TEST_F(MsgHandlerTest, HandleSegMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("seg Team1");

    auto gameOverInfo = gameInfos->isGameOver();
    EXPECT_TRUE(gameOverInfo.first);
    EXPECT_EQ(gameOverInfo.second, "Team1");
}

// Test Server Message
TEST_F(MsgHandlerTest, HandleSmgMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("smg Hello from server");

    SUCCEED();
}

// Test Unknown Command Message
TEST_F(MsgHandlerTest, HandleSucMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("suc");

    SUCCEED();
}

// Test Bad Parameters Message
TEST_F(MsgHandlerTest, HandleSbpMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("sbp");

    SUCCEED();
}

// Test Empty Message
TEST_F(MsgHandlerTest, HandleEmptyMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("");

    SUCCEED();
}

// Test Invalid Message
TEST_F(MsgHandlerTest, HandleInvalidMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("invalid message format");

    SUCCEED();
}

// Test Invalid Player Number Format
TEST_F(MsgHandlerTest, HandleInvalidPlayerNumberFormat) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("ppo invalid_player 5 10 2");

    SUCCEED();
}

// Test Invalid Coordinates
TEST_F(MsgHandlerTest, HandleInvalidCoordinates) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("ppo #1 -5 -10 2");

    SUCCEED();
}

// Test Message with Network Exception
TEST_F(MsgHandlerTest, HandleMessageWithNetworkException) {
    EXPECT_CALL(*mockCommunication, disconnect())
        .Times(testing::AtLeast(0));

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    EXPECT_NO_THROW(msgHandler->handleMessage("WELCOME"));
}

// Test Start and Stop
TEST_F(MsgHandlerTest, StartAndStop) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->stop();
    msgHandler->start();
    msgHandler->stop();
}

// Test Multiple Team Names
TEST_F(MsgHandlerTest, HandleMultipleTeamNames) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("tna Team1");
    msgHandler->handleMessage("tna Team2");
    msgHandler->handleMessage("tna Team3");

    auto teamNames = gameInfos->getTeamNames();
    EXPECT_EQ(teamNames.size(), 3);
    EXPECT_EQ(teamNames[0], "Team1");
    EXPECT_EQ(teamNames[1], "Team2");
    EXPECT_EQ(teamNames[2], "Team3");
}

// Test Invalid Resource IDs
TEST_F(MsgHandlerTest, HandleInvalidResourceIds) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pdr #1 -1");
    msgHandler->handleMessage("pgt #1 999");

    SUCCEED();
}

// Test Edge Cases for Numbers
TEST_F(MsgHandlerTest, HandleEdgeCaseNumbers) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("msz 99 99");

    msgHandler->handleMessage("sgt 0");

    SUCCEED();
}

// ============= ADDITIONAL COMPREHENSIVE TESTS FOR MAX COVERAGE =============

// Test Invalid Message Formats
TEST_F(MsgHandlerTest, HandleInvalidMessageFormats) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("msz abc def");
    msgHandler->handleMessage("msz 10");
    msgHandler->handleMessage("msz");
    msgHandler->handleMessage("msz 0 10");
    msgHandler->handleMessage("msz 10 0");

    msgHandler->handleMessage("sgt abc");
    msgHandler->handleMessage("sgt");
    msgHandler->handleMessage("sgt -1");

    msgHandler->handleMessage("sst abc");
    msgHandler->handleMessage("sst");
    msgHandler->handleMessage("sst -1");
    msgHandler->handleMessage("sst 0");

    SUCCEED();
}

// Test Invalid BCT Message Formats
TEST_F(MsgHandlerTest, HandleInvalidBctMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("bct 5 8 abc 1 2 3 4 5 6");
    msgHandler->handleMessage("bct 5 8 10 -1 2 3 4 5 6");
    msgHandler->handleMessage("bct -1 8 10 1 2 3 4 5 6");
    msgHandler->handleMessage("bct 5 -1 10 1 2 3 4 5 6");
    msgHandler->handleMessage("bct 5 8");
    msgHandler->handleMessage("bct");

    SUCCEED();
}

// Test Invalid TNA Message Formats
TEST_F(MsgHandlerTest, HandleInvalidTnaMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("tna");
    msgHandler->handleMessage("tna ");

    SUCCEED();
}

// Test Invalid PNW Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPnwMessages) {
    gameInfos->updateTeamName("Team1");
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pnw #1 abc 10 2 3 Team1");
    msgHandler->handleMessage("pnw #1 5 abc 2 3 Team1");
    msgHandler->handleMessage("pnw #1 5 10 abc 3 Team1");
    msgHandler->handleMessage("pnw #1 5 10 2 abc Team1");
    msgHandler->handleMessage("pnw #1 -1 10 2 3 Team1");
    msgHandler->handleMessage("pnw #1 5 -1 2 3 Team1");
    msgHandler->handleMessage("pnw #1 5 10 0 3 Team1");
    msgHandler->handleMessage("pnw #1 5 10 5 3 Team1");
    msgHandler->handleMessage("pnw #1 5 10 2 0 Team1");
    msgHandler->handleMessage("pnw #1 5 10 2 3");
    msgHandler->handleMessage("pnw #1 5 10 2 3 NonExistentTeam");
    msgHandler->handleMessage("pnw 1 5 10 2 3 Team1");
    msgHandler->handleMessage("pnw");

    SUCCEED();
}

// Test Invalid PPO Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPpoMessages) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("ppo #1 abc 15 3");
    msgHandler->handleMessage("ppo #1 10 abc 3");
    msgHandler->handleMessage("ppo #1 10 15 abc");
    msgHandler->handleMessage("ppo #1 -1 15 3");
    msgHandler->handleMessage("ppo #1 10 -1 3");
    msgHandler->handleMessage("ppo #1 10 15 0");
    msgHandler->handleMessage("ppo #1 10 15 5");
    msgHandler->handleMessage("ppo 1 10 15 3");
    msgHandler->handleMessage("ppo #abc 10 15 3");
    msgHandler->handleMessage("ppo");

    SUCCEED();
}

// Test Invalid PLV Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPlvMessages) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("plv #1 abc");
    msgHandler->handleMessage("plv #1 0");
    msgHandler->handleMessage("plv #1 -1");
    msgHandler->handleMessage("plv 1 5");
    msgHandler->handleMessage("plv #abc 5");
    msgHandler->handleMessage("plv");

    SUCCEED();
}

// Test Invalid PIN Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPinMessages) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pin #1 abc 10 10 1 2 3 4 5 6");
    msgHandler->handleMessage("pin #1 5 abc 10 1 2 3 4 5 6");
    msgHandler->handleMessage("pin #1 5 10 abc 1 2 3 4 5 6");
    msgHandler->handleMessage("pin #1 -1 10 10 1 2 3 4 5 6");
    msgHandler->handleMessage("pin #1 5 -1 10 1 2 3 4 5 6");
    msgHandler->handleMessage("pin #1 5 10 -1 1 2 3 4 5 6");
    msgHandler->handleMessage("pin 1 5 10 10 1 2 3 4 5 6");
    msgHandler->handleMessage("pin #abc 5 10 10 1 2 3 4 5 6");
    msgHandler->handleMessage("pin");

    SUCCEED();
}

// Test Invalid PEX Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPexMessages) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pex 1");
    msgHandler->handleMessage("pex #abc");
    msgHandler->handleMessage("pex");

    SUCCEED();
}

// Test Invalid PBC Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPbcMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pbc 1 Hello World");
    msgHandler->handleMessage("pbc #abc Hello World");
    msgHandler->handleMessage("pbc #1");
    msgHandler->handleMessage("pbc");

    SUCCEED();
}

// Test Invalid PIC Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPicMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pic abc 10 2 #1 #2 #3");
    msgHandler->handleMessage("pic 5 abc 2 #1 #2 #3");
    msgHandler->handleMessage("pic 5 10 abc #1 #2 #3");
    msgHandler->handleMessage("pic -1 10 2 #1 #2 #3");
    msgHandler->handleMessage("pic 5 -1 2 #1 #2 #3");
    msgHandler->handleMessage("pic 5 10 0 #1 #2 #3");
    msgHandler->handleMessage("pic 5 10 2 1 #2 #3");
    msgHandler->handleMessage("pic 5 10 2 #abc #2 #3");
    msgHandler->handleMessage("pic");

    SUCCEED();
}

// Test Invalid PIE Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPieMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pie abc 10 1");
    msgHandler->handleMessage("pie 5 abc 1");
    msgHandler->handleMessage("pie 5 10 abc");
    msgHandler->handleMessage("pie -1 10 1");
    msgHandler->handleMessage("pie 5 -1 1");
    msgHandler->handleMessage("pie 5 10 -1");
    msgHandler->handleMessage("pie");

    SUCCEED();
}

// Test Invalid PFK Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPfkMessages) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pfk 1");
    msgHandler->handleMessage("pfk #abc");
    msgHandler->handleMessage("pfk");

    SUCCEED();
}

// Test Invalid PDR Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPdrMessages) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    // Invalid formats
    msgHandler->handleMessage("pdr #1 abc");
    msgHandler->handleMessage("pdr #1 7");
    msgHandler->handleMessage("pdr 1 0");
    msgHandler->handleMessage("pdr #abc 0");
    msgHandler->handleMessage("pdr");

    SUCCEED();
}

// Test Invalid PGT Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPgtMessages) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pgt #1 abc");
    msgHandler->handleMessage("pgt #1 7");
    msgHandler->handleMessage("pgt 1 1");
    msgHandler->handleMessage("pgt #abc 1");
    msgHandler->handleMessage("pgt");

    SUCCEED();
}

// Test Invalid PDI Message Formats
TEST_F(MsgHandlerTest, HandleInvalidPdiMessages) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("pdi 1");
    msgHandler->handleMessage("pdi #abc");
    msgHandler->handleMessage("pdi");

    SUCCEED();
}

// Test Invalid ENW Message Formats
TEST_F(MsgHandlerTest, HandleInvalidEnwMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("enw #1 #2 abc 10 Team1");
    msgHandler->handleMessage("enw #1 #2 5 abc Team1");
    msgHandler->handleMessage("enw #1 #2 -1 10 Team1");
    msgHandler->handleMessage("enw #1 #2 5 -1 Team1");
    msgHandler->handleMessage("enw 1 #2 5 10 Team1");
    msgHandler->handleMessage("enw #1 2 5 10 Team1");
    msgHandler->handleMessage("enw #abc #2 5 10 Team1");
    msgHandler->handleMessage("enw #1 #abc 5 10 Team1");
    msgHandler->handleMessage("enw #1 #2 5 10");
    msgHandler->handleMessage("enw");

    SUCCEED();
}

// Test Invalid EBO Message Formats
TEST_F(MsgHandlerTest, HandleInvalidEboMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("ebo 1");
    msgHandler->handleMessage("ebo #abc");
    msgHandler->handleMessage("ebo");

    SUCCEED();
}

// Test Invalid EDI Message Formats
TEST_F(MsgHandlerTest, HandleInvalidEdiMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("edi 1");
    msgHandler->handleMessage("edi #abc");
    msgHandler->handleMessage("edi");

    SUCCEED();
}

// Test Invalid SEG Message Formats
TEST_F(MsgHandlerTest, HandleInvalidSegMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("seg");

    SUCCEED();
}

// Test Invalid SMG Message Formats
TEST_F(MsgHandlerTest, HandleInvalidSmgMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("smg");

    SUCCEED();
}

// Test Invalid SUC Message Formats
TEST_F(MsgHandlerTest, HandleInvalidSucMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("suc extra");

    SUCCEED();
}

// Test Invalid SBP Message Formats
TEST_F(MsgHandlerTest, HandleInvalidSbpMessages) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->handleMessage("sbp extra");

    SUCCEED();
}

// Test Valid Resource IDs Range
TEST_F(MsgHandlerTest, HandleValidResourceIdsRange) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    for (int i = 0; i <= 6; i++) {
        msgHandler->handleMessage("pdr #1 " + std::to_string(i));
        msgHandler->handleMessage("pgt #1 " + std::to_string(i));
    }

    SUCCEED();
}

// Test Edge Cases for Orientations
TEST_F(MsgHandlerTest, HandleEdgeCaseOrientations) {
    gameInfos->updateTeamName("Team1");
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    for (int orientation = 1; orientation <= 4; orientation++) {
        msgHandler->handleMessage("pnw #" + std::to_string(orientation) + " 5 10 " + 
                                  std::to_string(orientation) + " 3 Team1");
    }

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players.size(), 4);

    SUCCEED();
}

// Test Threading Functionality
TEST_F(MsgHandlerTest, HandleThreadingFunctionality) {
    EXPECT_CALL(*mockCommunication, hasMessages())
        .WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*mockCommunication, isConnected())
        .WillRepeatedly(testing::Return(true));

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    msgHandler->stop();

    SUCCEED();
}

// Test SecurityActualization Function
TEST_F(MsgHandlerTest, HandleSecurityActualization) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player1(1, 5, 5, 1, 3, "Team1");
    zappy::structs::Player player2(2, 10, 10, 2, 4, "Team1");
    gameInfos->addPlayer(player1);
    gameInfos->addPlayer(player2);

    EXPECT_CALL(*mockCommunication, sendMessage(testing::HasSubstr("msz")))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(*mockCommunication, sendMessage(testing::HasSubstr("tna")))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(*mockCommunication, sendMessage(testing::HasSubstr("sgt")))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(*mockCommunication, sendMessage(testing::HasSubstr("mct")))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(*mockCommunication, sendMessage(testing::HasSubstr("ppo")))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(*mockCommunication, sendMessage(testing::HasSubstr("plv")))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(*mockCommunication, sendMessage(testing::HasSubstr("pin")))
        .Times(testing::AtLeast(0));

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    SUCCEED();
}

// Test Network Exception Handling in SecurityActualization
TEST_F(MsgHandlerTest, HandleNetworkExceptionInSecurityActualization) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    EXPECT_CALL(*mockCommunication, sendMessage(testing::_))
        .WillRepeatedly(testing::Throw(Exceptions::NetworkException("Network error")));

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    SUCCEED();
}

// Test Welcome Message Empty
TEST_F(MsgHandlerTest, HandleWelcomeMessageEmpty) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("");

    SUCCEED();
}

// Test Constructor Destructor Coverage
TEST_F(MsgHandlerTest, HandleConstructorDestructor) {
    {
        auto localMsgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    }

    SUCCEED();
}

// Test Multiple Start/Stop Calls
TEST_F(MsgHandlerTest, HandleMultipleStartStopCalls) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    msgHandler->start();
    msgHandler->start();
    msgHandler->start();

    msgHandler->stop();
    msgHandler->stop();
    msgHandler->stop();

    SUCCEED();
}

// Test All Resource Names Coverage
TEST_F(MsgHandlerTest, HandleAllResourceNamesCoverage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);

    std::vector<std::string> resources = {"0", "1", "2", "3", "4", "5", "6"};
    for (const auto& resource : resources) {
        msgHandler->handleMessage("pdr #1 " + resource);
        msgHandler->handleMessage("pgt #1 " + resource);
    }

    SUCCEED();
}
