#include "MediaFactory.h"
#include "Exceptions.h"
#include "IMediaExtractor.h"

std::shared_ptr<IPlayable> MediaFactory::build(const File& file) {
    if (!getFiletypes().count(file.getType())) {
        throw FileException("unsupported type");
    }
    std::shared_ptr<IMediaExtractor> extractor = getFiletypes().at(file.getType());

    return extractor->extract(file);
}

void MediaFactory::registerFiletype(const std::string& name, const std::shared_ptr<IMediaExtractor>& extractor) {
    getFiletypes().emplace(name, extractor);
}

std::map<std::string, std::shared_ptr<IMediaExtractor>>& MediaFactory::getFiletypes() {
    static std::map<std::string, std::shared_ptr<IMediaExtractor>> filetypes{};
    return filetypes;
}
