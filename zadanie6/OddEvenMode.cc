#include "OddEvenMode.h"

std::vector<std::shared_ptr<IPlayable>> OddEvenMode::createOrder(std::vector<std::shared_ptr<IPlayable>> elements) {
    return elements;
}

std::shared_ptr<IPlayMode> createOddEvenMode() {
    return std::make_shared<OddEvenMode>();
}