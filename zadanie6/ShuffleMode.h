#ifndef PLAYER_SHUFFLEMODE_H
#define PLAYER_SHUFFLEMODE_H


#include "IPlayMode.h"

class ShuffleMode : IPlayMode {
public:
    std::vector<std::shared_ptr<IPlayable>> createOrder(std::vector<std::shared_ptr<IPlayable>> elements);
};


#endif //PLAYER_SHUFFLEMODE_H
