#ifndef PLAYER_IMEDIAEXTRACTOR_H
#define PLAYER_IMEDIAEXTRACTOR_H

#include "Media.h"
#include "File.h"
#include <memory>

class IMediaExtractor {
public:
    virtual std::shared_ptr<Media> extract(const File &file) = 0;
};

#endif //PLAYER_IMEDIAEXTRACTOR_H
