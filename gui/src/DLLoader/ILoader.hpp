/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ILoader
*/

#ifndef ILoader_HPP_
#define ILoader_HPP_


class ILoader {
    public:
        ~ILoader() = default;

        virtual void *Open(const char *path, int flag) = 0;
        virtual void *Symbol(const char *symbolName) = 0;
        virtual int Close() = 0;
        virtual const char *Error() = 0;
        virtual void *getHandler() const = 0;

    protected:
    private:
};

#endif /* !ILoader_HPP_ */
