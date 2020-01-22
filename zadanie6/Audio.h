#ifndef PLAYER_AUDIO_H
#define PLAYER_AUDIO_H

#include "Media.h"
#include "IMediaExtractor.h"

class Audio : public Media {
public:
    explicit Audio(const File& file);
    void play() override;

private:
    std::string artist;
};

class AudioExtractor : public IMediaExtractor {
public:
    std::shared_ptr<Media> extract(const File &file) override;
    ~AudioExtractor() override = default;
};

#endif //PLAYER_AUDIO_H
