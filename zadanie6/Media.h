#ifndef PLAYER_MEDIA_H
#define PLAYER_MEDIA_H

#include "Playable.h"
#include "File.h"

class Media : public Playable {
protected:
    std::string content;
    std::string title;

    bool content_valid();
};

class Audio : public Media {
public:
    explicit Audio(const File& file);
    void play() override;

private:
    std::string artist;
};

class Video : public Media {
public:
    explicit Video(const File& file);
    void play() override;

private:
    std::string year;
};


#endif //PLAYER_MEDIA_H
