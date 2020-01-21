#ifndef PLAYER_MEDIAEXCEPTION_H
#define PLAYER_MEDIAEXCEPTION_H

#include "PlayerException.h"

class MediaException : public PlayerException {
public:
    explicit MediaException(const char *str) : PlayerException(str) {}
};

#endif //PLAYER_MEDIAEXCEPTION_H
