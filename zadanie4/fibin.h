#ifndef FIBIN_H
#define FIBIN_H

#include <cstdint>

//TODO: check if variable name is good
static constexpr uint64_t Var(const char* name) {
    uint64_t res = 0;

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

template <uint64_t N>
struct Fib {
    template <typename ValueType>
    static const ValueType value = Fib<N - 1>::template value<ValueType> + Fib<N - 2>::template value<ValueType>;
};

template <>
struct Fib<0> {
    template <typename ValueType>
    static const ValueType value = ValueType(0);
};

template <>
struct Fib<1> {
    template <typename ValueType>
    static const ValueType value = ValueType(1);
};

#endif
