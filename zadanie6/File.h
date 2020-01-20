#ifndef PLAYER_FILE_H
#define PLAYER_FILE_H

#include <map>
#include <string>

class File {
public:
    explicit File(std::string content);
    const std::string& getType() const;
    const std::map<std::string, std::string>& getAttributes() const;
    const std::string& getContent() const;

private:
    std::string type;
    std::map<std::string, std::string> attributes;
    std::string content;
};

#endif  // PLAYER_FILE_H
