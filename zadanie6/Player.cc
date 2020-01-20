#include "Player.h"
#include "Audio.h"

std::shared_ptr<IPlayable> Player::openFile(File file) {
    std::shared_ptr<IPlayable> ret = std::make_shared<Audio>();
    return ret;
}

std::shared_ptr<Playlist> Player::createPlaylist(std::string name) {
    return std::make_shared<Playlist>(name);
}
