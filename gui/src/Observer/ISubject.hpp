/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** ISubject
*/

#ifndef ISUBJECT_HPP_
#define ISUBJECT_HPP_

#include <vector>
#include <memory>
#include <string>
#include "IObserver.hpp"

class ISubject {
    public:
        virtual ~ISubject() = default;
        virtual void addObserver(std::shared_ptr<IObserver> observer) = 0;
        virtual void removeObserver(std::shared_ptr<IObserver> observer) = 0;
        virtual void notifyObservers() = 0;
        virtual void notifyGameEvent(GameEventType eventType, const std::string& teamName) = 0;

    protected:
        std::vector<std::weak_ptr<IObserver>> _observers;
};

#endif /* !ISUBJECT_HPP_ */
