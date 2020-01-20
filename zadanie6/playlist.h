#ifndef PLAYER_PLAYLIST_H
#define PLAYER_PLAYLIST_H

#include "Audio.h"
#include "Video.h"
#include "File.h"
#include "IPlayMode.h"

IPlayMode createSequenceMode();
IPlayMode createShuffleMode(int seed);
IPlayMode createOddEvenMode();

#endif //PLAYER_PLAYLIST_H
