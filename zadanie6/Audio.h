#ifndef PLAYER_AUDIO_H
#define PLAYER_AUDIO_H

#include "IMediaExtractor.h"

class Audio : public IPlayable {
public:
    explicit Audio(const std::map<std::string, std::string>& attrs, const std::string& content);
    void play() override;

private:
    std::string title;
    std::string artist;
    std::string content;
};

class AudioExtractor : public IMediaExtractor {
public:
    std::shared_ptr<IPlayable> extract(const File& file) override;
    ~AudioExtractor() override = default;
};

#endif  // PLAYER_AUDIO_H
