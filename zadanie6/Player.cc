#include "Player.h"
#include "Media.h"
#include "PlayerException.h"
#include "FileException.h"
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
    return std::make_shared<Playlist>(name);
}

