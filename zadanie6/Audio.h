#ifndef PLAYER_AUDIO_H
#define PLAYER_AUDIO_H

#include "IPlayable.h"

class Audio : public IPlayable {
public:
    void play() override;
};
#endif //PLAYER_AUDIO_H
