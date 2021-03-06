#ifndef PLAYER_IPLAYMODE_H
#define PLAYER_IPLAYMODE_H

#include <memory>
#include <vector>
#include "IPlayable.h"

class IPlayMode {
public:
    virtual std::vector<std::shared_ptr<IPlayable>> createOrder(std::vector<std::shared_ptr<IPlayable>> elements) = 0;
    virtual ~IPlayMode() = default;
};

#endif  // PLAYER_IPLAYMODE_H
