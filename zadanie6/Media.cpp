#include "Media.h"
#include "Exceptions.h"


bool Media::content_valid() {
    for (char ch : content) {
        bool is_special = false;
        if (ch == '.' || ch == ',' || ch == '!' || ch == '?' || ch == '\'' || ch == ':' || ch == ';' || ch == '-')
            is_special = true;

        if (!std::isalnum(ch) && !std::isblank(ch) && !is_special)
            return false;
    }

    return true;

}