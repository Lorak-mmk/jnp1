#ifndef PLAYER_AUDIO_H
#define PLAYER_AUDIO_H

#include <string>
#include "File.h"
#include "Playable.h"

class Audio : public Playable {
public:
    explicit Audio(const File& file);
    void play() override;

private:
    std::string artist;
    std::string title;
    std::string content;
};

#endif  // PLAYER_AUDIO_H
