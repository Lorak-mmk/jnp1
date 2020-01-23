#include "Playlist.h"
#include <iostream>
#include <utility>
#include "Exceptions.h"
#include "SequenceMode.h"

Playlist::Playlist(std::string name) : name(std::move(name)), elements(), mode(std::make_shared<SequenceMode>()) {}

bool Playlist::contains(IPlayable *other) {
    for (const auto &element : elements) {
        if (element->contains(other)) {
            return true;
        }
    }
    return false;
}

void Playlist::add(const std::shared_ptr<IPlayable> &element) {
    if (element->contains(this)) {
        throw PlaylistException("Cycle detected (playlist add)");
    }
    elements.push_back(element);
}

void Playlist::add(const std::shared_ptr<IPlayable> &element, size_t position) {
    if (elements.size() < position) {
        throw PlaylistException("Index out of range (playlist add)");
    }
    if (element->contains(this)) {
        throw PlaylistException("Cycle detected (playlist add)");
    }
    elements.insert(elements.begin() + position, element);
}

void Playlist::remove() {
    if (elements.empty()) {
        throw PlaylistException("Index out of range (remove() on empty playlist)");
    }

    elements.pop_back();
}

void Playlist::remove(size_t position) {
    if (elements.size() <= position) {
        throw PlaylistException("Index out of range (playlist remove)");
    }
    elements.erase(elements.begin() + position);
}

void Playlist::setMode(std::shared_ptr<IPlayMode> new_mode) noexcept {
    this->mode = std::move(new_mode);
}

void Playlist::play() {
    std::cout << "Playlist [" << name << "]\n";
    for (const std::shared_ptr<IPlayable> &element : mode->createOrder(elements)) {
        element->play();
    }
}
