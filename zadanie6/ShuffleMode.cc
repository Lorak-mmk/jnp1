#include "ShuffleMode.h"

std::vector<std::shared_ptr<IPlayable>> ShuffleMode::createOrder(std::vector<std::shared_ptr<IPlayable>> elements) {
    return elements;
}

std::shared_ptr<IPlayMode> createShuffleMode(int seed) {
    return std::make_shared<ShuffleMode>();
}