#ifndef PLAYER_MEDIAFACTORY_H
#define PLAYER_MEDIAFACTORY_H

#include <memory>
#include "File.h"
#include "IPlayable.h"

class MediaFactory {
public:
    static std::shared_ptr<IPlayable> build(const File &file);
};

#endif  // PLAYER_MEDIAFACTORY_H
