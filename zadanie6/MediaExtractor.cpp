#include "MediaExtractor.h"

std::shared_ptr<Media> VideoExtractor::extract(const File &file) {
    auto foo = std::make_shared<Video>(file);
    return std::static_pointer_cast<Media>(foo);
}

std::shared_ptr<Media> AudioExtractor::extract(const File &file) {
    auto foo = std::make_shared<Audio>(file);
    return std::static_pointer_cast<Media>(foo);
}
