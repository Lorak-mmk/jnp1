#include "Player.h"
#include "Audio.h"
#include "Video.h"
#include "PlayerException.h"

// TODO: some form of lookup table to support more types
std::shared_ptr<Playable> Player::openFile(File file) {
    if(file.getType() == "audio") {
        return std::make_shared<Audio>(file);
    } else if(file.getType() == "video") {
        return std::make_shared<Video>(file);
    }
    // TODO: throw appriopiate error
    throw PlayerException();
}

std::shared_ptr<Playlist> Player::createPlaylist(std::string name) {
    return std::make_shared<Playlist>(name);
}
