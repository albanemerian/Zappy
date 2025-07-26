/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** MsgHandler
*/

#ifndef MSGHANDLER_HPP_
#define MSGHANDLER_HPP_

#include <memory>
#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <string>

#include "../Game/GameInfos.hpp"
#include "../Communication/ICommunication.hpp"
#include "../Utils/Constants.hpp"

class MsgHandler {
    public:
        MsgHandler(std::shared_ptr<GameInfos> gameInfos,
            std::shared_ptr<ICommunication> communication);
        ~MsgHandler();

        void start();
        void stop();

        void handleMessage(const std::string& message);

    protected:
        void messageLoop();

        bool handleWelcomeMessage(const std::string& message);
        bool handleMszMessage(const std::string& message);
        bool handleBctMessage(const std::string& message);
        bool handleTnaMessage(const std::string& message);
        bool handlePnwMessage(const std::string& message);
        bool handlePpoMessage(const std::string& message);
        bool handlePlvMessage(const std::string& message);
        bool handlePinMessage(const std::string& message);
        bool handlePexMessage(const std::string& message);
        bool handlePbcMessage(const std::string& message);
        bool handlePicMessage(const std::string& message);
        bool handlePieMessage(const std::string& message);
        bool handlePfkMessage(const std::string& message);
        bool handlePdrMessage(const std::string& message);
        bool handlePgtMessage(const std::string& message);
        bool handlePdiMessage(const std::string& message);
        bool handleEnwMessage(const std::string& message);
        bool handleEboMessage(const std::string& message);
        bool handleEdiMessage(const std::string& message);
        bool handleSgtMessage(const std::string& message);
        bool handleSstMessage(const std::string& message);
        bool handleSegMessage(const std::string& message);
        bool handleSmgMessage(const std::string& message);
        bool handleSucMessage(const std::string& message);
        bool handleSbpMessage(const std::string& message);

    private:
        std::thread _thread;
        std::atomic<bool> _running;
        std::mutex _mutex;
        std::condition_variable _condition;

        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<ICommunication> _communication;
        std::mutex _gameInfosMutex;

        std::map<std::string, std::function<bool(const std::string&)>> _messageHandlers;
};

#endif /* !MSGHANDLER_HPP_ */
