/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** entryPoint
*/

#include <memory>
#include "../IDisplay.hpp"
#include "../DLLoader/LoaderType.hpp"
#include "Raylib.hpp"

extern "C" {
    std::shared_ptr<IDisplay> create(void) {
        return std::make_shared<Raylib>();
    }
    ModuleType_t getType(void) {
        return ModuleType_t::DISPLAY_MODULE;
    }
}
