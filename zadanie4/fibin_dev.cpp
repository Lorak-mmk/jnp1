#include <iostream>
#include "fibin.h"

int main() {
    std::cout << Fib<64>::value<uint64_t> << "\n";
    constexpr uint64_t x = Var("abcd");
    std::cout << x << "\n";
    
    //static_assert(Fibin<int>::eval<Let<Var(""), Lit<Fib<1>>, Ref<Var("")> > >());
    //static_assert(Fibin<int>::eval<Let<Var("1111111"), Lit<Fib<1>>, Ref<Var("1111111")> > >());
    //static_assert(Fibin<int>::eval<Let<Var("!@"), Lit<Fib<1>>, Ref<Var("!@")> > >());
    //static_assert(Fibin<int>::eval<Let<Var(nullptr), Lit<Fib<1>>, Ref<Var(nullptr)> > >());

    // Fibin<const char*>::eval<Fib<1>>();

    static_assert(Fibin<int>::eval<If<Lit<True>, Lit<Fib<10>>, Lit<Fib<4>>>>() == 55);

    // if (if (true) return Fib<10>; else return Fib<4>;) return 1; else return 0;
    static_assert(
        Fibin<int>::eval<If<Eq<If<Lit<True>, Lit<Fib<10>>, Lit<Fib<4>>>, Lit<Fib<10>>>, Lit<Fib<1>>, Lit<Fib<0>>>>() ==
        1);

    static_assert(Fibin<int>::eval<Let<Var("A"), Lit<Fib<1>>, Ref<Var("a")>>>() == 1);

    static_assert(Fibin<int>::eval<Sum<Lit<Fib<0>>, Lit<Fib<1>>, Lit<Fib<3>>>>() == 3);
    static_assert(Fibin<int>::eval<Inc1<Lit<Fib<0>>>>() == 1);

    static_assert(Fibin<int>::eval<Invoke<Lambda<Var("x"), Ref<Var("x")>>, Lit<Fib<3>>>>() == 2);

    static_assert(
        Fibin<uint64_t>::eval<Invoke<Let<Var("x"), Lit<Fib<0>>, Lambda<Var("x"), Ref<Var("x")>>>, Lit<Fib<1>>>>() == 1);

    static_assert(Fibin<uint64_t>::eval<If<Eq<Inc1<Lit<Fib<1>>>, Lit<Fib<3>>>, Lit<Fib<1>>, Lit<Fib<0>>>>() == 1);
}
