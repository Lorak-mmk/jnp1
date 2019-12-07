#include <iostream>
#include "fibin.h"

int main() {
    std::cout << Fib<64>::value<uint64_t> << "\n";
    constexpr uint64_t x = Var("abcd");
    std::cout << x << "\n";

    // Fibin<const char*>::eval<Fib<1>>();

    static_assert(Fibin<int>::eval<If<Lit<True>, Lit<Fib<10>>, Lit<Fib<4>>>>() == 55);
}
