#include "ShuffleMode.h"
#include <algorithm>

ShuffleMode::ShuffleMode(unsigned seed) : engine(seed) {}

std::vector<std::shared_ptr<Playable>> ShuffleMode::createOrder(std::vector<std::shared_ptr<Playable>> elements) {
    std::vector<std::shared_ptr<Playable>> result(elements);
    std::shuffle(result.begin(), result.end(), engine);
    return result;
}

std::shared_ptr<IPlayMode> createShuffleMode(unsigned seed) {
    return std::make_shared<ShuffleMode>(seed);
}