#include "Player.h"

std::shared_ptr<IPlayable> Player::openFile(File file) {
    return std::shared_ptr<IPlayable>();
}

Playlist Player::createPlaylist(std::string name) {
    return Playlist();
}
