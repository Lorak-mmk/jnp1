#include "Playlist.h"
#include "SequenceMode.h"
#include "Exceptions.h"
#include "Audio.h"
#include "Video.h"
#include <iostream>

Playlist::Playlist(std::string name) try : name(std::move(name)), elements(), mode(std::make_shared<SequenceMode>()) {}
    catch (const std::exception &e) {
        throw PlaylistException("Bad playlist initialisation");
    }

void Playlist::add(const std::shared_ptr<IPlayable> &element) {
    try {
        elements.push_back(element);
    }
    catch (...) {
        throw PlaylistException("Bad insertion to playlist");
    }
}

void Playlist::add(const std::shared_ptr<IPlayable> &element, size_t position) {
    try {
        elements.insert(elements.begin() + position, element);
    }
    catch (...) {
        throw PlaylistException("Bad insertion to playlist");
    }
}

void Playlist::remove() {
    try {
        if (elements.empty())
            throw PlaylistException("prevent undefined behaviour");

        elements.pop_back();
    }
    catch (...) {
        throw PlaylistException("Bad removal from playlist");
    }
}

void Playlist::remove(size_t position) {
    try {
        elements.erase(elements.begin() + position);
    }
    catch (...) {
        throw PlaylistException("Bad removal from playlist");
    }
}

void Playlist::setMode(std::shared_ptr<IPlayMode> new_mode) noexcept {
    this->mode = std::move(new_mode);
}

void Playlist::play() {
    try {
        auto used = std::set<std::shared_ptr<IPlayable>>();
        if (is_cycle(used))
            throw PlaylistException("Cycle detected");

        std::cout << "Playlist [" << name << "]\n";
        for (const std::shared_ptr<IPlayable> &element : mode->createOrder(elements))
            element->play();
    }
    catch (...) {
        throw PlaylistException("Playlist::play terminated");
    }
}

bool Playlist::is_cycle(std::set<std::shared_ptr<IPlayable>> &used) {
    for (const auto& top : elements) {
        if (std::dynamic_pointer_cast<Playlist>(top) != nullptr) {
            if (used.find(top) != used.end())
                return true;

            used.insert(top);
            auto ptr = std::dynamic_pointer_cast<Playlist>(top);
            return ptr->is_cycle(used);
        }
        else if (std::dynamic_pointer_cast<Audio>(top) == nullptr && std::dynamic_pointer_cast<Video>(top) == nullptr) {
            throw PlaylistException("Unknown type");
        }
    }

    return false;
}


