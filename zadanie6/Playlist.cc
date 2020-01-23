#include "Playlist.h"
#include <iostream>
#include <utility>
#include "Audio.h"
#include "Exceptions.h"
#include "SequenceMode.h"
#include "Video.h"

Playlist::Playlist(std::string name) try : name(std::move(name)), elements(), mode(std::make_shared<SequenceMode>()) {
} catch (const std::exception &e) {
    throw PlaylistException("Bad playlist initialisation");
}

bool Playlist::contains(IPlayable *other) {
    for (const auto &element : elements) {
        if (element->contains(other))
            return true;
    }
    return false;
}

void Playlist::add(const std::shared_ptr<IPlayable> &element) {
    try {
        elements.push_back(element);
    } catch (...) {
        throw PlaylistException("Bad insertion to playlist");
    }
}

void Playlist::add(const std::shared_ptr<IPlayable> &element, size_t position) {
    try {
        elements.insert(elements.begin() + position, element);
    } catch (...) {
        throw PlaylistException("Bad insertion to playlist");
    }
}

void Playlist::remove() {
    if (elements.empty()) {
        throw PlaylistException("Index out of range (remove() on empty playlist)");
    }

    elements.pop_back();
}

void Playlist::remove(size_t position) {
    if (elements.size() < position) {
        throw PlaylistException("Index out of range");
    }
    try {
        elements.erase(elements.begin() + position);
    } catch (...) {
        throw PlaylistException("Bad removal from playlist");
    }
}

void Playlist::setMode(std::shared_ptr<IPlayMode> new_mode) noexcept {
    this->mode = std::move(new_mode);
}

void Playlist::play() {
    try {
        std::cout << "Playlist [" << name << "]\n";
        for (const std::shared_ptr<IPlayable> &element : mode->createOrder(elements))
            element->play();
    } catch (...) {
        throw PlaylistException("Playlist::play terminated (IO error?)");
    }
}
