#include <iostream>
#include "Audio.h"
#include "Exceptions.h"

Audio::Audio(const File& file) {
    if (file.getType() != "audio")
        throw MediaException("unsupported type passed to Audio");

    artist = file.getAttrs().at("artist");  // at() throws exception if particular key doesn't exist.
    title = file.getAttrs().at("title");
    content = file.getContent();

    if (!content_valid())
        throw MediaException("corrupt content");
}

void Audio::play() {
    std::cout << "Song [" << artist << ", " << title << "]: " << content << "\n";
}

std::shared_ptr<Media> AudioExtractor::extract(const File &file) {
    return std::make_shared<Audio>(file);
}