#ifndef PLAYER_IMEDIAEXTRACTOR_H
#define PLAYER_IMEDIAEXTRACTOR_H

#include "File.h"
#include "IPlayable.h"
#include <memory>

class IMediaExtractor {
public:
    virtual std::shared_ptr<IPlayable> extract(const File &file) = 0;
    virtual ~IMediaExtractor() = default;
};

#endif //PLAYER_IMEDIAEXTRACTOR_H
