#ifndef PLAYER_VIDEO_H
#define PLAYER_VIDEO_H

#include "IMediaExtractor.h"
#include "IPlayable.h"

class Video : public IPlayable {
public:
    explicit Video(const std::map<std::string, std::string>& attrs, const std::string& content);
    void play() override;

private:
    std::string title;
    std::string year;
    std::string content;
};

class VideoExtractor : public IMediaExtractor {
public:
    std::shared_ptr<IPlayable> extract(const File &file) override;
    ~VideoExtractor() = default;
};


#endif //PLAYER_VIDEO_H
