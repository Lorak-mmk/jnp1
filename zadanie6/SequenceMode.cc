#include "SequenceMode.h"
#include "Exceptions.h"

std::vector<std::shared_ptr<IPlayable>> SequenceMode::createOrder(std::vector<std::shared_ptr<IPlayable>> elements) {
    return elements;
}

std::shared_ptr<IPlayMode> createSequenceMode() {
    try {
        return std::make_shared<SequenceMode>();
    }
    catch (...) {
        throw ModeException("Exception while creating mode");
    }
}