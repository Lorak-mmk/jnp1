#include <iostream>
#include <algorithm>
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

    bool is_number(const std::string& s) {
        return !s.empty() && std::find_if(s.begin(),
                s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    }
}

Video::Video(const File &file) {
    if (file.getType() != "video")
        throw MediaException("unsupported type passed to Video");

    year = file.getAttrs().at("year");
    title = file.getAttrs().at("title");
    content = ::ROT13(file.getContent());


    if (!content_valid() || !is_number(year))
        throw MediaException("corrupt content");
}

void Video::play() {
    std::cout << "Movie [" << title << ", " << year << "]: " << content << "\n";
}

std::shared_ptr<Media> VideoExtractor::extract(const File &file) {
    return std::make_shared<Video>(file);
}

