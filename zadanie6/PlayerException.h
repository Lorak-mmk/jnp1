#ifndef PLAYER_PLAYEREXCEPTION_H
#define PLAYER_PLAYEREXCEPTION_H

#include <exception>
#include <string>

class PlayerException : public std::exception {
public:
    explicit PlayerException(const char *str) : msg(str) {}
    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }
protected:
    std::string msg;
};

#endif  // PLAYER_PLAYEREXCEPTION_H
