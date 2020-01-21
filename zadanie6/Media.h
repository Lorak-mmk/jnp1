#ifndef PLAYER_MEDIA_H
#define PLAYER_MEDIA_H

#include <string>
#include "Playable.h"

class Media : public Playable {
protected:
    std::string content;
    std::string title;

    bool content_valid();
};

#endif //PLAYER_MEDIA_H
