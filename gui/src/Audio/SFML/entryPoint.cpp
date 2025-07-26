/*
** EPITECH PROJECT, 2025
** gui
** File description:
** entryPoint.cpp
*/

#include <memory>
#include "../../IDisplay.hpp"
#include "../../DLLoader/LoaderType.hpp"
#include "SFMLAudio.hpp"


extern "C" {
    std::shared_ptr<IAudio> create(void) {
        return std::make_shared<SFMLAudio>();
    }
    ModuleType_t getType(void) {
        return ModuleType_t::AUDIO_MODULE;
    }
}
