#include "OddEvenMode.h"

std::vector<std::shared_ptr<IPlayable>> OddEvenMode::createOrder(std::vector<std::shared_ptr<IPlayable>> elements) {
    std::vector<std::shared_ptr<IPlayable>> result;
    for (size_t i = 0; i < elements.size(); i += 2)
        result.push_back(elements[i]);

    for (size_t i = 1; i < elements.size(); i += 2)
        result.push_back(elements[i]);

    return result;
}

std::shared_ptr<IPlayMode> createOddEvenMode() {
    return std::make_shared<OddEvenMode>();
}