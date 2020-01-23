#include "ShuffleMode.h"
#include <algorithm>
#include "Exceptions.h"

ShuffleMode::ShuffleMode(unsigned seed) : engine(seed) {}

std::vector<std::shared_ptr<IPlayable>> ShuffleMode::createOrder(std::vector<std::shared_ptr<IPlayable>> elements) {
    std::vector<std::shared_ptr<IPlayable>> result(elements);
    std::shuffle(result.begin(), result.end(), engine);
    return result;
}

std::shared_ptr<IPlayMode> createShuffleMode(unsigned seed) {
    try {
        return std::make_shared<ShuffleMode>(seed);
    } catch (...) {
        throw ModeException("Exception while creating mode");
    }
}