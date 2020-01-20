#ifndef PLAYER_IPLAYMODE_H
#define PLAYER_IPLAYMODE_H

#include <vector>
#include "IPlayable.h"

class IPlayMode {
public:
    virtual std::vector<IPlayable> createOrder(std::vector<IPlayable> elements) = 0;
};


#endif //PLAYER_IPLAYMODE_H
