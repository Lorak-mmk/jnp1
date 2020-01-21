#include "MediaFactory.h"
#include "MediaExtractor.h"
#include "FileException.h"

std::shared_ptr<Media> MediaFactory::build(const File &file) {
    try {
        std::shared_ptr<MediaExtractor> extractor;

        if (file.getType() == "audio") {
            auto foo = std::make_shared<AudioExtractor>();
            extractor = std::static_pointer_cast<MediaExtractor>(foo);
        } else if (file.getType() == "video") {
            auto foo = std::make_shared<VideoExtractor>();
            extractor = std::static_pointer_cast<MediaExtractor>(foo);
        } else {
            throw FileException("unsupported type");
        }

        return extractor->extract(file);
    }
    catch (const std::exception &e) {
        throw MediaException(e.what());
    }
}

