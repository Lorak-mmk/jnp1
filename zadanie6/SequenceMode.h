#ifndef PLAYER_SEQUENCEMODE_H
#define PLAYER_SEQUENCEMODE_H

#include "IPlayMode.h"

class SequenceMode : public IPlayMode {
public:
    std::vector<std::shared_ptr<IPlayable>> createOrder(std::vector<std::shared_ptr<IPlayable>> elements) override;
};

std::shared_ptr<IPlayMode> createSequenceMode();

#endif  // PLAYER_SEQUENCEMODE_H
