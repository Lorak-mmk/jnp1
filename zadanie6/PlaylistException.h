#ifndef PLAYER_PLAYLISTEXCEPTION_H
#define PLAYER_PLAYLISTEXCEPTION_H

#include "PlayerException.h"

class PlaylistException : public PlayerException {
public:
    explicit PlaylistException(const char *str) : PlayerException(str) {}
};

#endif //PLAYER_PLAYLISTEXCEPTION_H
