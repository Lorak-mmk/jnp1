#include "Video.h"
#include <algorithm>
#include <iostream>
#include "Exceptions.h"
#include "MediaFactory.h"

namespace {
bool content_valid(const std::string& content) {
    for (char ch : content) {
        bool is_special = false;
        if (ch == '.' || ch == ',' || ch == '!' || ch == '?' || ch == '\'' || ch == ':' || ch == ';' || ch == '-') {
            is_special = true;
        }

        if (!std::isalnum(ch) && !std::isblank(ch) && !is_special) {
            return false;
        }
    }

    return true;
}

std::string ROT13(const std::string& str) {
    std::string transcript = str;
    int diff = 13;

    for (size_t i = 0; i <= transcript.length(); i++) {
        if (std::isalnum(transcript[i])) {  // if it's not, character is not changed.
            if (std::isupper(transcript[i])) {
                if (transcript[i] < 'N') {
                    transcript[i] = transcript[i] + diff;
                } else {
                    transcript[i] = transcript[i] - diff;
                }
            } else if (std::islower(transcript[i])) {
                if (transcript[i] < 'n') {
                    transcript[i] = transcript[i] + diff;
                } else {
                    transcript[i] = transcript[i] - diff;
                }
            }
        }
    }

    return transcript;
}

bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(*s.begin() == '-' ? s.begin() + 1 : s.begin(), s.end(),
                                      [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

class InitModule {
public:
    InitModule() {
        MediaFactory::registerFiletype("video", std::make_shared<VideoExtractor>());
    }
};

InitModule init;
}  // namespace

Video::Video(const std::map<std::string, std::string>& attrs, const std::string& content) {
    if (!attrs.count("year") || !attrs.count("title")) {
        throw MediaException("(Video)Required metadata fields not found");
    }
    year = attrs.at("year");
    title = attrs.at("title");
    this->content = ::ROT13(content);

    if (!::content_valid(this->content) || !is_number(year)) {
        throw MediaException("Invalid content");
    }
}

void Video::play() {
    std::cout << "Movie [" << title << ", " << year << "]: " << content << "\n";
}

std::shared_ptr<IPlayable> VideoExtractor::extract(const File& file) {
    return std::make_shared<Video>(file.getAttrs(), file.getContent());
}
