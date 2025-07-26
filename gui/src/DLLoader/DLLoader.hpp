/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP_
#define DLLOADER_HPP_

#include <dlfcn.h>
#include <iostream>
#include <ostream>
#include <memory>
#include "ILoader.hpp"
#include "../Audio/IAudio.hpp"
#include "../IDisplay.hpp"

template <typename T>

class DLLoader : public ILoader {
    private:
        void *_handler = nullptr;

    public:
        ~DLLoader() = default;

        void *getHandler() const override {
            return _handler;
        };
        void *Open(const char *path, int flag = RTLD_LAZY) override {
            _handler = dlopen(path, flag);
            return _handler;
        };
        void *Symbol(const char *symbolName) override {
            void *symbol = dlsym(_handler, symbolName);
            const char *error = dlerror();
            if (error) {
                std::cerr << "dlerror: " << error << std::endl;
                return nullptr;
            }
            return symbol;
        };
        T getSymbol(const char *symbolName) {
            return reinterpret_cast<T>(dlsym(_handler, symbolName));
        };
        int Close() override{
            if (_handler == nullptr)
                return -1;
            return dlclose(_handler);
        };
        const char *Error() override {
            return dlerror();
        };
};

typedef std::shared_ptr<IDisplay> (*createGraphicFunc_t)();
typedef std::shared_ptr<IAudio> (*createAudioFunc_t)();

#endif /* !DLLOADER_HPP_ */
