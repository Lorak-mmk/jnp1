#include "Audio.h"
#include <iostream>

static const std::string title_key = "title";
static const std::string artist_key = "artist";

Audio::Audio(const File& file) {
    const std::map<std::string, std::string>& attrs = file.getAttributes();
    artist = attrs.at(artist_key);
    title = attrs.at(title_key);
    content = file.getContent();
}

void Audio::play() {
    std::cout << "Song [" << artist << ", " << title << "]: " << content << "\n";
}
