#include "SequenceMode.h"

std::vector<std::shared_ptr<IPlayable>> SequenceMode::createOrder(std::vector<std::shared_ptr<IPlayable>> elements) {
    return elements;
}

std::shared_ptr<IPlayMode> createSequenceMode() {
    return std::make_shared<SequenceMode>();
}