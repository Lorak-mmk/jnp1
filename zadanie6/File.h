#ifndef PLAYER_FILE_H
#define PLAYER_FILE_H

#include <map>
#include <string>
#include <vector>

class File {
public:
    explicit File(const std::string &content);
    [[nodiscard]] const std::string& getType() const;
    [[nodiscard]] const std::map<std::string, std::string>& getAttrs() const;
    [[nodiscard]] const std::string& getContent() const;

private:
    std::string type;
    std::string content;
    std::map<std::string, std::string> attrs;

    class FileExtractor {
    public:
        virtual void extract(File *f, const std::vector<std::string> &data) = 0;
        virtual ~FileExtractor() = default;
        friend class File;  // changes File's private params during its initialisation.
    };

    class AudioExtractor : public FileExtractor {
        void extract(File *f, const std::vector<std::string> &data) override;
        ~AudioExtractor() override = default;
    };

    class VideoExtractor : public FileExtractor {
        void extract(File *f, const std::vector<std::string> &data) override;
        ~VideoExtractor() override = default;
    };

    static FileExtractor *chooseExtractor(const std::vector<std::string> &data);
};

#endif  // PLAYER_FILE_H
