#ifndef PLAYER_PLAYER_H
#define PLAYER_PLAYER_H

#include "IPlayable.h"
#include "File.h"
#include "Playlist.h"

class Player {
public:
    IPlayable openFile(File file);
    Playlist createPlaylist(std::string name);
};


#endif //PLAYER_PLAYER_H
