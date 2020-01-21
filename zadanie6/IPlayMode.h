#ifndef PLAYER_IPLAYMODE_H
#define PLAYER_IPLAYMODE_H

#include <memory>
#include <vector>
#include "Playable.h"

class IPlayMode {
public:
    virtual std::vector<std::shared_ptr<Playable>> createOrder(std::vector<std::shared_ptr<Playable>> elements) = 0;
};

#endif  // PLAYER_IPLAYMODE_H
