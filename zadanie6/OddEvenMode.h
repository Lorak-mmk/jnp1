#ifndef PLAYER_ODDEVENMODE_H
#define PLAYER_ODDEVENMODE_H


#include "IPlayMode.h"

class OddEvenMode : public IPlayMode {
public:
    std::vector<std::shared_ptr<IPlayable>> createOrder(std::vector<std::shared_ptr<IPlayable>> elements);
};

std::shared_ptr<IPlayMode> createOddEvenMode();

#endif //PLAYER_ODDEVENMODE_H
