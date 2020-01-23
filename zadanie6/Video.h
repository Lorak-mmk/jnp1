#ifndef PLAYER_VIDEO_H
#define PLAYER_VIDEO_H

#include "Media.h"
#include "IMediaExtractor.h"

class Video : public Media {
public:
    explicit Video(const File& file);
    void play() override;

private:
    std::string year;
};

class VideoExtractor : public IMediaExtractor {
public:
    std::shared_ptr<Media> extract(const File &file) override;
    ~VideoExtractor() = default;
};


#endif //PLAYER_VIDEO_H
