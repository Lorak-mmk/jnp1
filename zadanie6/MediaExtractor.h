#ifndef PLAYER_MEDIAEXTRACTOR_H
#define PLAYER_MEDIAEXTRACTOR_H

#include "Media.h"
#include "File.h"
#include <memory>

class MediaExtractor {
public:
    virtual std::shared_ptr<Media> extract(const File &file) = 0;
    virtual ~MediaExtractor() = default;
};

class AudioExtractor : public MediaExtractor {
public:
    std::shared_ptr<Media> extract(const File &file) override;
    ~AudioExtractor() override = default;
};

class VideoExtractor : public MediaExtractor {
public:
    std::shared_ptr<Media> extract(const File &file) override;
    ~VideoExtractor() override = default;
};

#endif //PLAYER_MEDIAEXTRACTOR_H
