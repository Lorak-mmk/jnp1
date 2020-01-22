#include <iostream>
#include "Video.h"
#include "Exceptions.h"

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

Video::Video(const File &file) {
    if (file.getType() != "video")
        throw MediaException("unsupported type passed to Video");

    //TODO: throw exception if attrs don't exist
    year = file.getAttrs().at("year");
    title = file.getAttrs().at("title");
    content = ::ROT13(file.getContent());

    if (!content_valid())
        throw MediaException("corrupt content");
}

void Video::play() {
    std::cout << "Movie [" << title << ", " << year << "]: " << content << "\n";
}

std::shared_ptr<Media> VideoExtractor::extract(const File &file) {
    auto foo = std::make_shared<Video>(file);
    return std::static_pointer_cast<Media>(foo);
}

