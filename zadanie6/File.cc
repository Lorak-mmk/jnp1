#include "FileException.h"
#include "File.h"
#include <sstream>
#include <memory>

namespace {
    std::vector<std::string> split(const std::string &str, char delimiter) {
        std::vector<std::string> data;
        std::stringstream iss(str);
        std::string auxiliary;

        while (std::getline(iss, auxiliary, delimiter))
            data.push_back(auxiliary);

        return data;
    }

    std::string ROT13(const std::string &str) {
        std::string transcript = str;

        for (size_t i = 0; i <= transcript.length(); i++) {
            if (std::isalnum(transcript[i])) {                  // if it's not, character is not changed.
                if (transcript[i] < 91 && transcript[i] > 64) {
                    if(transcript[i] < 78)
                        transcript[i] = transcript[i] + 13;
                    else
                        transcript[i] = transcript[i] - 13;
                }

                if (transcript[i] < 123 && transcript[i] > 96) {
                    if(transcript[i] < 110)
                        transcript[i] = transcript[i] + 13;
                    else
                        transcript[i] = transcript[i] - 13;
                }
            }
        }

        return transcript;
    }

    std::string merge_meta(const std::vector<std::string> &meta) {
        std::string result;
        for (auto it = std::next(meta.begin()); it != meta.end(); ++it)
            result += (it == std::next(meta.begin()) ? *it : ":" + *it);

        return result;
    }
}


File::File(const std::string &str) : type(), attrs(), content() {
    try {
        const std::vector<std::string> data = ::split(str, '|');
        auto extractor = std::unique_ptr<FileExtractor> (chooseExtractor(data)); //extractor is deleted while leaving {} braces.
        extractor->extract(this, data);
    }
    catch (...) {
        throw FileException();
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

File::FileExtractor *File::chooseExtractor(const std::vector<std::string> &data) {
    if (data.size() < 4UL)
        throw FileException();

    if (data.front() == "audio")
        return new AudioExtractor();
    else if (data.front() == "video")
        return new VideoExtractor();
    else
        throw FileException();
}

void File::AudioExtractor::extract(File *f, const std::vector<std::string> &data) {
    f->type = data.front();    // here data.size is >= 4
    f->content = data.back();

    for (auto it = std::next(data.begin()); it != std::prev(data.end()); std::advance(it, 1)) {
        std::vector<std::string> meta = ::split(*it, ':');
        f->attrs[meta[0]] = merge_meta(meta);   // "artist:john:lennon" is converted to "artist": "john:lennon"
    }

    if (f->attrs.find("artist") == f->attrs.end() || f->attrs.find("title") == f->attrs.end())
        throw FileException();
}

void File::VideoExtractor::extract(File *f, const std::vector<std::string> &data) {
    f->type = data.front();          // here data.size is >= 4
    f->content = ROT13(data.back());

    for (auto it = std::next(data.begin()); it != std::prev(data.end()); std::advance(it, 1)) {
        std::vector<std::string> meta = ::split(*it, ':');

        f->attrs[meta[0]] = merge_meta(meta);    // "artist:john:lennon" is converted to "artist": "john:lennon"
    }

    if (f->attrs.find("year") == f->attrs.end() || f->attrs.find("title") == f->attrs.end())
        throw FileException();
}
