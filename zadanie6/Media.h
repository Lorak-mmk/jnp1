#ifndef PLAYER_MEDIA_H
#define PLAYER_MEDIA_H

#include "IPlayable.h"
#include <string>

class Media : public IPlayable {
protected:
    std::string content;
    std::string title;

    bool content_valid();
};



#endif //PLAYER_MEDIA_H
