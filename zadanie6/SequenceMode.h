#ifndef PLAYER_SEQUENCEMODE_H
#define PLAYER_SEQUENCEMODE_H


#include "IPlayMode.h"

class SequenceMode : IPlayMode {
public:
    std::vector<std::shared_ptr<IPlayable>> createOrder(std::vector<std::shared_ptr<IPlayable>> elements);
};


#endif //PLAYER_SEQUENCEMODE_H
