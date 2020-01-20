#ifndef PLAYER_ODDEVENMODE_H
#define PLAYER_ODDEVENMODE_H


#include "IPlayMode.h"

class OddEvenMode : IPlayMode {
public:
    std::vector<std::shared_ptr<IPlayable>> createOrder(std::vector<std::shared_ptr<IPlayable>> elements);
};


#endif //PLAYER_ODDEVENMODE_H
