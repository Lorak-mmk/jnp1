#ifndef FIBIN_H
#define FIBIN_H

#include <cstddef>

static constexpr size_t Var(const char* name) {
    size_t res = 0;

    while (*name) {
        char ch = *name;
        name++;

        if (ch >= 'A' && ch <= 'Z') ch = 'a' + (ch - 'A');

        res *= 256;
        res += ch;
    }

    return res;
}

#endif
