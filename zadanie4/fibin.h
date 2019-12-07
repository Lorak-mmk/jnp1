#ifndef FIBIN_H
#define FIBIN_H

#include <cstdint>

// Returns 0 if name is wrong
constexpr uint64_t Var(const char* name) {
    uint64_t res = 0;
    
    std::size_t length = 0;
    while (*name) {
        char ch = *(name++);
        if(! (('1' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))) return 0;
        length++;
        
        if (ch >= 'A' && ch <= 'Z') ch = 'a' + (ch - 'A');

        res *= 256;
        res += ch;
        
    }
    
    if(length < 1 || 6 < length) return 0;
    return res;
}

struct True {};
struct False {};

template <uint64_t N>
struct Fib {
    template <typename ValueType>
    static constexpr ValueType value = Fib<N - 1>::template value<ValueType> + Fib<N - 2>::template value<ValueType>;
};

template <>
struct Fib<0> {
    template <typename ValueType>
    static constexpr ValueType value = ValueType(0);
};

template <>
struct Fib<1> {
    template <typename ValueType>
    static constexpr ValueType value = ValueType(1);
};

#endif
