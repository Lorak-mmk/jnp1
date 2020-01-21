#include "SequenceMode.h"

std::vector<std::shared_ptr<Playable>> SequenceMode::createOrder(std::vector<std::shared_ptr<Playable>> elements) {
    return elements;
}

std::shared_ptr<IPlayMode> createSequenceMode() {
    return std::make_shared<SequenceMode>();
}