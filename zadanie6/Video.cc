#include <iostream>
#include "Video.h"
#include "FileException.h"

Video::Video(const File &file) {
    if (file.getType() != "video")
        throw FileException();

    year = file.getAttrs().at("year");
    title = file.getAttrs().at("title");
    content = file.getContent();

    if (!content_valid())
        throw FileException();
}

void Video::play() {
    std::cout << "Movie [" << title << ", " << year << "]: " << content << "\n";
}
