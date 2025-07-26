/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>

#include "Client.hpp"
#include "../Communication/Communication.hpp"
#include "../CLI/CLI.hpp"

Client::Client(int ac, const char *const *av)
{
    initialize(ac, av);

    std::cout << colors::T_GREEN << "Client initialized with configuration: "
              << "Port: " << _config.port
              << ", Hostname: " << _config.hostname
              << colors::RESET << std::endl;

    _communication = std::make_shared<Communication>(_config.port, _config.hostname);
    _gameInfos = std::make_shared<GameInfos>(_communication);
    _msgHandler = std::make_unique<MsgHandler>(_gameInfos, _communication);

    if (!_communication->isConnected())
        return;
    this->_retrieveAllAudioSharedLib();
    this->_retrieveAllGraphicSharedLib();
    this->_createGUI();
    if (!this->_gui) {
        std::cerr << "No gui lib found" << std::endl;
        exit(84);
    }
    _guiObserver = std::make_shared<GuiObserver>(_gui);
    _gameInfos->addObserver(_guiObserver);
    _gui->run();
}

Client::~Client()
{
}

void Client::stop()
{
    if (_gui)
        _gui->stop();

    if (_communication)
        _communication->disconnect();
}

bool Client::_checkIfCorrectModuleType(ModuleType_t type, const std::string &path)
{
    auto dlLoader = DLLoader<std::shared_ptr<IDisplay>>();
    if (path.empty())
        throw std::runtime_error(path + ": Lib input empty");
    dlLoader.Open(path.c_str());
    if (!dlLoader.getHandler())
        throw std::runtime_error(path + ": Failed to open library: " +
            std::string(dlerror()));

    auto getTypeFunc = reinterpret_cast<getTypeFunc_t>(
        dlLoader.Symbol("getType"));
    bool result = getTypeFunc && getTypeFunc() == type;
    dlLoader.Close();
    return result;
}

void Client::_retrieveAllGraphicSharedLib(const std::string &path)
{
    try {
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            if (entry.path().extension() == ".so") {
                if (this->_checkIfCorrectModuleType(DISPLAY_MODULE, entry.path().string()))
                    this->_pathLibGraphic.push_back(entry.path().string());
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
}

void Client::_retrieveAllAudioSharedLib(const std::string &path)
{
    try {
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            if (entry.path().extension() == ".so") {
                if (this->_checkIfCorrectModuleType(AUDIO_MODULE, entry.path().string()))
                    this->_pathLibAudio.push_back(entry.path().string());
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
}

void Client::_createGUI()
{
    for (const auto &libGraphic : this->_pathLibGraphic) {
        try {
            for (const auto &libAudio : this->_pathLibAudio) {
                try {
                    this->_gui = std::make_unique<GUI>(this->_gameInfos,
                        libGraphic, libAudio);
                } catch (Exceptions::ModuleAudioError &e) {
                    this->_gui = nullptr;
                    std::cerr << "libAudio: " << libAudio << " is not compatible" << std::endl;
                    continue;
                }
                break;
            }
        } catch (Exceptions::ModuleGraphicError &e) {
            this->_gui = nullptr;
            std::cerr << "libGraphic: " << libGraphic << " is not compatible" << std::endl;
            continue;
        }
        break;
    }
}

void Client::initialize(int ac, const char *const *av)
{
    CLI cli(ac, av);

    _config = cli.parseArguments(ac, av);
}
