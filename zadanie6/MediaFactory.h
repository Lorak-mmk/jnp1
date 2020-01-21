#ifndef PLAYER_MEDIAFACTORY_H
#define PLAYER_MEDIAFACTORY_H

#include "Media.h"
#include "File.h"
#include <memory>

class MediaFactory {
public:
    static std::shared_ptr<Media> build(const File &file);
};

#endif //PLAYER_MEDIAFACTORY_H
