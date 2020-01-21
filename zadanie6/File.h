#ifndef PLAYER_FILE_H
#define PLAYER_FILE_H

#include <map>
#include <string>

class File {
public:
    explicit File(const std::string &content);
    [[nodiscard]] const std::string& getType() const;
    [[nodiscard]] const std::map<std::string, std::string>& getAttrs() const;
    [[nodiscard]] const std::string& getContent() const;

private:
    const std::string type = "type";
    const std::string content = "content";
    std::map<std::string, std::string> attrs;
};

#endif  // PLAYER_FILE_H
