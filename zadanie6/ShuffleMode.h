#ifndef PLAYER_SHUFFLEMODE_H
#define PLAYER_SHUFFLEMODE_H

#include "IPlayMode.h"

class ShuffleMode : public IPlayMode {
public:
    std::vector<std::shared_ptr<IPlayable>> createOrder(std::vector<std::shared_ptr<IPlayable>> elements);
};

std::shared_ptr<IPlayMode> createShuffleMode(int seed);

#endif  // PLAYER_SHUFFLEMODE_H
