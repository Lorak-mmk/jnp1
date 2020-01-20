#include "File.h"

File::File(std::string content) : type(), attributes(), content() {
    (void)content;
}

const std::string& File::getType() const {
    return type;
}

const std::map<std::string, std::string>& File::getAttributes() const {
    return attributes;
}

const std::string& File::getContent() const {
    return content;
}
