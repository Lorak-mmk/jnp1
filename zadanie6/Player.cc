#include "Player.h"
#include "Media.h"
#include "Exceptions.h"
#include "MediaFactory.h"

std::shared_ptr<Media> Player::openFile(const File& file) {
    try {
        return MediaFactory::build(file);
    }
    catch (const std::exception &e) {
        throw MediaException(e.what());
    }
}

std::shared_ptr<Playlist> Player::createPlaylist(const std::string &name) {
    try {
        return std::make_shared<Playlist>(name);
    }
    catch  (const std::exception &e) {
        throw PlaylistException(e.what());
    }
}
