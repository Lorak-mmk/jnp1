#include "Media.h"
#include "MediaException.h"
#include <iostream>

namespace {
    std::string ROT13(const std::string &str) {
        std::string transcript = str;
        int diff = 13;

        for (size_t i = 0; i <= transcript.length(); i++) {
            if (std::isalnum(transcript[i])) {                  // if it's not, character is not changed.
                if (std::isupper(transcript[i])) {
                    if(transcript[i] < 'N')
                        transcript[i] = transcript[i] + diff;
                    else
                        transcript[i] = transcript[i] - diff;
                }

                if (std::islower(transcript[i])) {
                    if(transcript[i] < 'n')
                        transcript[i] = transcript[i] + diff;
                    else
                        transcript[i] = transcript[i] - diff;
                }
            }
        }

        return transcript;
    }
}

bool Media::content_valid() {
    for (char ch : content) {
        bool is_special = false;
        if (ch == '.' || ch == ',' || ch == '!' || ch == '?' || ch == '\'' || ch == ':' || ch == ';' || ch == '-')
            is_special = true;

        if (!std::isalnum(ch) && !std::isblank(ch) && !is_special)
            return false;
    }

    return true;
}

Audio::Audio(const File& file) {
    if (file.getType() != "audio")
        throw MediaException("unsupported type passed to Audio");

    artist = file.getAttrs().at("artist");
    title = file.getAttrs().at("title");
    content = file.getContent();

    if (!content_valid())
        throw MediaException("corrupt content");
}

void Audio::play() {
    std::cout << "Song [" << artist << ", " << title << "]: " << content << "\n";
}

Video::Video(const File &file) {
    if (file.getType() != "video")
        throw MediaException("unsupported type passed to Video");

    year = file.getAttrs().at("year");
    title = file.getAttrs().at("title");
    content = ::ROT13(file.getContent());

    if (!content_valid())
        throw MediaException("corrupt content");
}

void Video::play() {
    std::cout << "Movie [" << title << ", " << year << "]: " << content << "\n";
}
