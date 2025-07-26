/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Subject
*/

#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <mutex>

#include "ISubject.hpp"

#ifndef SUBJECT_HPP_
#define SUBJECT_HPP_

class Subject : public ISubject {
    public:
        virtual ~Subject() = default;

        void addObserver(std::shared_ptr<IObserver> observer) override;

        void removeObserver(std::shared_ptr<IObserver> observer) override;

        void notifyObservers() override;

        void notifyGameEvent(GameEventType eventType, const std::string& teamName);

    private:
        std::vector<std::weak_ptr<IObserver>> _observers;
        mutable std::mutex _observersMutex;
};

#endif /* !SUBJECT_HPP_ */
