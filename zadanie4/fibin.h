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

struct True {};
struct False {};

template <size_t N>
struct Fib {
    // TODO iteracyjnie
    template <typename ValueType>
    static constexpr ValueType compute() {
        return Fib<N - 1>::template compute<ValueType>() + Fib<N - 2>::template compute<ValueType>();
    }
};

template <>
struct Fib<0> {
    template <typename ValueType>
    static constexpr ValueType compute() {
        return ValueType(0);
    }
};

template <>
struct Fib<1> {
    template <typename ValueType>
    static constexpr ValueType compute() {
        return ValueType(1);
    }
};

#endif
