#ifndef PLAYER_VIDEO_H
#define PLAYER_VIDEO_H

#include <string>
#include "Media.h"
#include "File.h"

class Video : public Media {
public:
    explicit Video(const File& file);
    void play() override;

private:
    std::string year;
};

#endif  // PLAYER_VIDEO_H
