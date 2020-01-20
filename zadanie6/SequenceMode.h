#ifndef PLAYER_SEQUENCEMODE_H
#define PLAYER_SEQUENCEMODE_H


#include "IPlayMode.h"

class SequenceMode : IPlayMode {
public:
    std::vector<IPlayable> createOrder(std::vector<IPlayable> elements);
};


#endif //PLAYER_SEQUENCEMODE_H
