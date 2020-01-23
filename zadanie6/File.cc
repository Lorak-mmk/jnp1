#include "File.h"
#include <sstream>
#include <vector>
#include "Exceptions.h"

namespace {
std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> data;
    std::stringstream iss(str);
    std::string auxiliary;

    while (std::getline(iss, auxiliary, delimiter))
        data.push_back(auxiliary);

    return data;
}

std::string merge_meta(const std::vector<std::string> &meta) {
    std::string result;
    for (auto it = std::next(meta.begin()); it != meta.end(); ++it)
        result += (it == std::next(meta.begin()) ? *it : ":" + *it);

    return result;
}
}  // namespace

File::File(const std::string &str) try : attrs(), type(), content() {
    const auto &data = ::split(str, '|');

    type = *data.begin();
    content = *std::prev(data.end());
    attrs = std::map<std::string, std::string>();

    for (auto it = std::next(data.begin()); it < std::prev(data.end()); ++it) {
        const auto &meta = ::split(*it, ':');
        if (meta.size() == 1)
            throw FileException("Invalid metadata (no value)");  // e.g "audio|key|content"

        attrs[meta[0]] = merge_meta(meta);  // "artist:john:lennon"  =>  "artist": "john:lennon"
    }
} catch (const std::exception &e) {  // Any subclass of std::exception (nothing else can be thrown here)
    throw FileException(e.what());
}

const std::string &File::getType() const {
    return type;
}

const std::map<std::string, std::string> &File::getAttrs() const {
    return attrs;
}

const std::string &File::getContent() const {
    return content;
}