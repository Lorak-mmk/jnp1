#include <iostream>
#include "Video.h"

static const std::string title_key = "title";
static const std::string year_key = "year";

Video::Video(const File &file) {
    const std::map<std::string, std::string>& attrs = file.getAttributes();
    title = attrs.at(title_key);
    year = attrs.at(year_key);
    content = file.getContent(); // TODO: ROT13
}

void Video::play() {
    std::cout << "Movie [" << title << ", " << year << "]: " << content << "\n";
}
