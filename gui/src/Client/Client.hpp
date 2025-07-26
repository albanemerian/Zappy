/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <memory>
#include <vector>
#include <filesystem>
#include <string>
#include "../Utils/Constants.hpp"
#include "../Communication/ICommunication.hpp"
#include "../Game/GameInfos.hpp"
#include "../Graphic/GUI.hpp"
#include "MsgHandler.hpp"
#include "../Observer/GuiObserver.hpp"
#include "DLLoader/LoaderType.hpp"
#include "../Observer/IObserver.hpp"

class Client {
    private:
        std::vector<std::string> _pathLibGraphic;
        std::vector<std::string> _pathLibAudio;
        zappy::structs::Config _config;

        std::shared_ptr<ICommunication> _communication;
        std::shared_ptr<GameInfos> _gameInfos;
        std::unique_ptr<MsgHandler> _msgHandler;
        std::shared_ptr<GUI> _gui;
        std::shared_ptr<GuiObserver> _guiObserver;

        bool _checkIfCorrectModuleType(ModuleType_t type, const std::string &path);
        void initialize(int ac, const char * const *av);
        void _retrieveAllGraphicSharedLib(const std::string &path = "gui/lib/");
        void _retrieveAllAudioSharedLib(const std::string &path = "gui/lib/");
    public:
        Client(int ac, const char *const *av);
        ~Client();

        void _createGUI();
        void stop();
};

#endif /* !CLIENT_HPP_ */
