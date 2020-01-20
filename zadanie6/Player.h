#ifndef PLAYER_PLAYER_H
#define PLAYER_PLAYER_H

#include <memory>
#include "IPlayable.h"
#include "File.h"
#include "Playlist.h"

class Player {
public:
    std::shared_ptr<IPlayable> openFile(File file);
    std::shared_ptr<Playlist> createPlaylist(std::string name);
};


#endif //PLAYER_PLAYER_H
