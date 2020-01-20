#include "Playlist.h"

#include <iostream>
#include <utility>
#include "SequenceMode.h"

Playlist::Playlist(std::string name) : name(std::move(name)), elements() {
    mode = std::make_shared<SequenceMode>();
}

void Playlist::add(const std::shared_ptr<IPlayable>& element) {
    elements.push_back(element);
}

void Playlist::add(const std::shared_ptr<IPlayable>& element, size_t position) {
    elements.insert(elements.begin() + position, element);
}

void Playlist::remove() {
    elements.pop_back();
}

void Playlist::remove(size_t position) {
    elements.erase(elements.begin() + position);
}

void Playlist::setMode(std::shared_ptr<IPlayMode> new_mode) {
    this->mode = std::move(new_mode);
}

void Playlist::play() {
    std::cout << "Playlist [" << name << "]\n";
    for (const std::shared_ptr<IPlayable>& element : mode->createOrder(elements)) {
        element->play();
    }
}
