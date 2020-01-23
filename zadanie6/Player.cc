#include "Player.h"
#include "Exceptions.h"
#include "MediaFactory.h"

std::shared_ptr<IPlayable> Player::openFile(const File &file) {
    return MediaFactory::build(file);
}

std::shared_ptr<Playlist> Player::createPlaylist(const std::string &name) {
    return std::make_shared<Playlist>(name);
}
