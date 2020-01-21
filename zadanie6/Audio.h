#ifndef PLAYER_AUDIO_H
#define PLAYER_AUDIO_H

#include <string>
#include "File.h"
#include "Media.h"

class Audio : public Media {
public:
    explicit Audio(const File& file);
    void play() override;

private:
    std::string artist;
};

#endif  // PLAYER_AUDIO_H
