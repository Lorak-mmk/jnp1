#include "Audio.h"
#include "FileException.h"
#include <iostream>

Audio::Audio(const File& file) {
    if (file.getType() != "audio")
        throw FileException();

    artist = file.getAttrs().at("artist");
    title = file.getAttrs().at("title");
    content = file.getContent();

    if (!content_valid())
        throw FileException();
}

void Audio::play() {
    std::cout << "Song [" << artist << ", " << title << "]: " << content << "\n";
}
