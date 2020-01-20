#ifndef PLAYER_ODDEVENMODE_H
#define PLAYER_ODDEVENMODE_H


#include "IPlayMode.h"

class OddEvenMode : IPlayMode {
public:
    std::vector<IPlayable> createOrder(std::vector<IPlayable> elements);
};


#endif //PLAYER_ODDEVENMODE_H
