#ifndef PLAYER_VIDEO_H
#define PLAYER_VIDEO_H

#include "IPlayable.h"

class Video : public IPlayable {
public:
    void play() override;
};
#endif  // PLAYER_VIDEO_H
