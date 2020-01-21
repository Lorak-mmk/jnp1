#ifndef PLAYER_SHUFFLEMODE_H
#define PLAYER_SHUFFLEMODE_H

#include <random>
#include "IPlayMode.h"

class ShuffleMode : public IPlayMode {
public:
    explicit ShuffleMode(unsigned seed);
    std::vector<std::shared_ptr<Playable>> createOrder(std::vector<std::shared_ptr<Playable>> elements);

private:
    std::default_random_engine engine;
};

std::shared_ptr<IPlayMode> createShuffleMode(unsigned seed);

#endif  // PLAYER_SHUFFLEMODE_H
