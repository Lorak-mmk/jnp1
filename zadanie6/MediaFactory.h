#ifndef PLAYER_MEDIAFACTORY_H
#define PLAYER_MEDIAFACTORY_H

#include <memory>
#include "File.h"
#include "IMediaExtractor.h"
#include "IPlayable.h"

class MediaFactory {
public:
    static std::shared_ptr<IPlayable> build(const File& file);
    static void registerFiletype(const std::string& name, const std::shared_ptr<IMediaExtractor>& extractor);

private:
    static std::map<std::string, std::shared_ptr<IMediaExtractor>>& getFiletypes();
};

#endif  // PLAYER_MEDIAFACTORY_H
