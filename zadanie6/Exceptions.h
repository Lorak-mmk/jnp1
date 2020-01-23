#ifndef PLAYER_EXCEPTIONS_H
#define PLAYER_EXCEPTIONS_H

#include <exception>
#include <string>

class PlayerException : public std::exception {
public:
    explicit PlayerException(const char *str) : msg(str) {}
    [[nodiscard]] const char *what() const noexcept override {
        return msg.c_str();
    }

protected:
    std::string msg;
};

class MediaException : public PlayerException {
public:
    explicit MediaException(const char *str) : PlayerException(str) {}
};

class PlaylistException : public PlayerException {
public:
    explicit PlaylistException(const char *str) : PlayerException(str) {}
};

class FileException : public MediaException {
public:
    explicit FileException(const char *str) : MediaException(str) {}
};

#endif  // PLAYER_EXCEPTIONS_H
