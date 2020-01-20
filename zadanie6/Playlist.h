#ifndef PLAYER_PLAYLIST_H
#define PLAYER_PLAYLIST_H


#include <cstddef>
#include <vector>
#include "IPlayable.h"
#include "IPlayMode.h"

class Playlist : IPlayable {
public:
    void add(IPlayable element);
    void add(IPlayable element, size_t position);
    void remove();
    void remove(size_t position);
    void setMode(IPlayMode mode);
    void play() override;
private:
    std::vector<IPlayable> elements;
};


#endif //PLAYER_PLAYLIST_H
