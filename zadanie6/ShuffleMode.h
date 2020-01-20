#ifndef PLAYER_SHUFFLEMODE_H
#define PLAYER_SHUFFLEMODE_H


#include "IPlayMode.h"

class ShuffleMode : IPlayMode {
public:
    std::vector<IPlayable> createOrder(std::vector<IPlayable> elements);
};


#endif //PLAYER_SHUFFLEMODE_H
