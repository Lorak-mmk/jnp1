#ifndef PLAYER_FILEEXCEPTION_H
#define PLAYER_FILEEXCEPTION_H

#include "MediaException.h"

class FileException : public MediaException {
public:
    explicit FileException(const char *str) : MediaException(str) {}
};

#endif //PLAYER_FILEEXCEPTION_H
