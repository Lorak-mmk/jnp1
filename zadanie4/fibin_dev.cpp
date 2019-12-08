#include <iostream>
#include "fibin.h"

int main() {
    std::cout << Fib<64>::value<uint64_t> << "\n";
    constexpr uint64_t x = Var("abcd");
    std::cout << x << "\n";

    // Fibin<const char*>::eval<Fib<1>>();

    static_assert(Fibin<int>::eval<If<Lit<True>, Lit<Fib<10>>, Lit<Fib<4>>>>() == 55);

    // if (true == false) return 1; else return 0;
    static_assert(Fibin<int>::eval<If<Eq<Lit<True>, Lit<False>>, Lit<Fib<1>>, Lit<Fib<0>>>>() == 0);
    // if (true == true) return 1; else return 0;
    static_assert(Fibin<int>::eval<If<Eq<Lit<True>, Lit<True>>, Lit<Fib<1>>, Lit<Fib<0>>>>() == 1);

    // if (if (true) return Fib<10>; else return Fib<4>;) return 1; else return 0;
    static_assert(
        Fibin<int>::eval<If<Eq<If<Lit<True>, Lit<Fib<10>>, Lit<Fib<4>>>, Lit<Fib<10>>>, Lit<Fib<1>>, Lit<Fib<0>>>>() ==
        1);
}
