#ifndef PLAYER_PLAYLIST_H
#define PLAYER_PLAYLIST_H

#include "Audio.h"
#include "Video.h"
#include "File.h"
#include "Player.h"
#include "IPlayMode.h"

std::shared_ptr<IPlayMode> createSequenceMode();
std::shared_ptr<IPlayMode> createShuffleMode(int seed);
std::shared_ptr<IPlayMode> createOddEvenMode();

#endif //PLAYER_PLAYLIST_H
