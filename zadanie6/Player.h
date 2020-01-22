#ifndef PLAYER_PLAYER_H
#define PLAYER_PLAYER_H

#include <memory>
#include "File.h"

#include "Playlist.h"
#include "Media.h"

class Player {
public:
    std::shared_ptr<Media> openFile(const File& file);
    std::shared_ptr<Playlist> createPlaylist(const std::string &name);
};

#endif  // PLAYER_PLAYER_H
