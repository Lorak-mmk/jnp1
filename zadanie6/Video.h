#ifndef PLAYER_VIDEO_H
#define PLAYER_VIDEO_H

#include <string>
#include "IPlayable.h"
#include "File.h"

class Video : public IPlayable {
public:
    explicit Video(const File& file);
    void play() override;
private:
    std::string title;
    std::string year;
    std::string content;
};
#endif  // PLAYER_VIDEO_H
