#include "MediaFactory.h"
#include "IMediaExtractor.h"
#include "Exceptions.h"

namespace {
    std::map<std::string, std::shared_ptr<IMediaExtractor>> filetypes{};
}

std::shared_ptr<IPlayable> MediaFactory::build(const File &file) {
    if(!filetypes.count(file.getType())) {
        throw FileException("unsupported type");
    }
    std::shared_ptr<IMediaExtractor> extractor = filetypes.at(file.getType());

    return extractor->extract(file);
}

void MediaFactory::registerFiletype(const std::string& name, const std::shared_ptr<IMediaExtractor>& extractor) {
    filetypes.emplace(name, extractor);
}

std::map<std::string, std::shared_ptr<IMediaExtractor>> MediaFactory::getFiletypes() {
    static std::map<std::string, std::shared_ptr<IMediaExtractor>> filetypes{};
    return filetypes;
}

