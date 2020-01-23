#include "MediaFactory.h"
#include "IMediaExtractor.h"
#include "Exceptions.h"
#include "Audio.h"
#include "Video.h"

std::shared_ptr<IPlayable> MediaFactory::build(const File &file) {
    try {
        std::shared_ptr<IMediaExtractor> extractor;

        if (file.getType() == "audio") {
            extractor = std::make_shared<AudioExtractor>();
        } else if (file.getType() == "video") {
            extractor = std::make_shared<VideoExtractor>();
        } else {
            throw FileException("unsupported type");
        }

        return extractor->extract(file);
    }
    catch (const std::exception &e) {
        throw MediaException(e.what());
    }
}

