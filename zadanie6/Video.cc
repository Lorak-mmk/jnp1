#include <iostream>
#include "Video.h"

static const std::string title_key = "title";
static const std::string year_key = "year";

Video::Video(const File &file) {
    try {
        const std::map<std::string, std::string>& attrs = file.getAttrs();
        title = attrs.at(title_key);    // may throw std::out_of_range
        year = attrs.at(year_key);      //
        content = file.getContent();    // TODO: ROT13
    }
    catch (std::out_of_range &exception) {

    }
}

void Video::play() {
    std::cout << "Movie [" << title << ", " << year << "]: " << content << "\n";
}
