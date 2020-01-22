#include <iostream>
#include "Audio.h"
#include "Exceptions.h"

Audio::Audio(const File& file) {
    if (file.getType() != "audio")
        throw MediaException("unsupported type passed to Audio");

    //TODO: throw exception if attrs don't exist
    artist = file.getAttrs().at("artist");
    title = file.getAttrs().at("title");
    content = file.getContent();

    if (!content_valid())
        throw MediaException("corrupt content");
}

void Audio::play() {
    std::cout << "Song [" << artist << ", " << title << "]: " << content << "\n";
}

std::shared_ptr<Media> AudioExtractor::extract(const File &file) {
    auto foo = std::make_shared<Audio>(file);
    return std::static_pointer_cast<Media>(foo);
}