#ifndef PLAYER_IPLAYMODE_H
#define PLAYER_IPLAYMODE_H

#include <vector>
#include <memory>
#include "IPlayable.h"

class IPlayMode {
public:
    virtual std::vector<std::shared_ptr<IPlayable>> createOrder(std::vector<std::shared_ptr<IPlayable>> elements) = 0;
};


#endif //PLAYER_IPLAYMODE_H
