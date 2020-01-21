#include "FileException.h"
#include "File.h"
#include <sstream>
#include <vector>

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
}

File::File(const std::string &str) {
    try {
        attrs = std::map<std::string, std::string>();
        const std::vector<std::string> data = ::split(str, '|');
        if (data.size() < 4UL)
            throw FileException("corrupt file");

        for (auto it = data.begin(); it != data.end(); ++it)
            if (it == data.begin()) {
                type = *it;
            } else if (it == std::prev(data.end())) {
                content = *it;
            } else {
                auto meta = ::split(*it, ':');
                attrs[meta[0]] = merge_meta(meta);    // "artist:john:lennon"  =>  "artist": "john:lennon"
            }
    }
    catch (const std::exception &e) {   // Any subclass of std::exception (nothing else can be thrown here)
        throw FileException(e.what());
    }
}

const std::string& File::getType() const {
    return type;
}

const std::map<std::string, std::string>& File::getAttrs() const {
    return attrs;
}

const std::string& File::getContent() const {
    return content;
}