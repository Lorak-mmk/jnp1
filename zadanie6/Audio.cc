#include "Audio.h"
#include "Exceptions.h"
#include <iostream>

namespace {
    bool content_valid(const std::string &content) {
        for (char ch : content) {
            bool is_special = false;
            if (ch == '.' || ch == ',' || ch == '!' || ch == '?' || ch == '\'' || ch == ':' || ch == ';' || ch == '-')
                is_special = true;

            if (!std::isalnum(ch) && !std::isblank(ch) && !is_special)
                return false;
        }

        return true;
    }
}

Audio::Audio(const std::map<std::string, std::string>& attrs, const std::string& content) {
    artist = attrs.at("artist");
    title = attrs.at("title");
    this->content = content;

    if (!content_valid(this->content))
        throw MediaException("corrupt content");
}

void Audio::play() {
    std::cout << "Song [" << artist << ", " << title << "]: " << content << "\n";
}

std::shared_ptr<IPlayable> AudioExtractor::extract(const File &file) {
    return std::make_shared<Audio>(file.getAttrs(), file.getContent());
}