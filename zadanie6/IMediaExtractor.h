#ifndef PLAYER_IMEDIAEXTRACTOR_H
#define PLAYER_IMEDIAEXTRACTOR_H

#include <memory>
#include "File.h"
#include "IPlayable.h"

class IMediaExtractor {
public:
    virtual std::shared_ptr<IPlayable> extract(const File &file) = 0;
    virtual ~IMediaExtractor() = default;
};

#endif  // PLAYER_IMEDIAEXTRACTOR_H
