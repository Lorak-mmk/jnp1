#include "fibo.h"

#include <cassert>
#include <iostream>

int main() {
    Fibo f;

    assert(f == Zero());
    assert(Fibo(f) == Zero());
    assert(Zero() < One());
    assert(Fibo("11") == Fibo("100"));
    auto f11 = (Fibo("1001"));
    auto f21 = (Fibo("10"));
    auto f31 = f21 + f11;
    assert((Fibo("1001") + Fibo("10")) == Fibo("1011"));
    assert((Fibo("1001") & Fibo("1100")) == Zero()); // 1100 == 10000
    assert((Fibo("1100") | Fibo("11")) == Fibo("10100")); // 1100 == 10000, 11 == 100
    assert((Fibo("1001") ^ Fibo("1010")) == Fibo("11"));
    assert((Fibo("101") << 3) == Fibo("101000"));

    f = One();
    f <<= 3;
    assert(f == Fibo("1000"));

    f = One();
    assert(f + Fibo("1") == Fibo("10"));
    assert(f == One());

    Fibo f1("101");
    Fibo f2 = Fibo("101");
    assert(f1 == f2);

    assert(Fibo("11").length() == 3); // 11 == 100

    std::cout << Fibo("11") << std::endl; // prints 100

    assert(Fibo(123) + Fibo(234) == Fibo(357));

    assert(Fibo(1001) + Fibo(2002) == Fibo(2900) + Fibo(103));

    assert(Fibo(100) < Fibo(100000));

    assert(18446744073709551615U + Fibo() == Fibo(18446744073709551615U));
    assert(18446744073709551615U + Zero() == Fibo(18446744073709551615U));
    assert(18446744073709551614U + One() == Fibo(18446744073709551615U));

    std::cout << "max uint64_t fibo representation: " << Fibo(18446744073709551615U) << std::endl;

//    Fibo bad_string("10101012");
//    Fibo bad_string("0001010101");
//    Fibo bad_string("0");
    std::cout << Fibo("1111111111111111111111111111") << std::endl;

    Fibo z = Zero();
    z += 4;
    std::cout << z << std::endl;
//    Zero() += 4;

    Fibo ft1(4);
    Fibo ft2(16);
    assert((ft1 < ft2));

    assert((Fibo() & Fibo("1111111111111111111111111111111")) == Fibo(0));
    std::cout << (Fibo("10101010101") & Fibo("100")) << std::endl;

    f1 += 2;
    f1 = f2 + 2;
    bool b = 2 < f2;

    // these should not compile
//    Fibo f3(true);
//    Fibo f4('a');
//    f1 += "10";
//    f1 = f2 + "10";
//    b = "10" < f2;
//    Fibo nulled(nullptr);
//    Fibo empty("");
//    f1 += true;
//    f1 = true  + f1;
//    Fibo fiboChar('w');
//    Fibo fiboBool(false);
//    char32_t c = '4';
//    Fibo fch32(c);
//    char16_t s = '4';
//    Fibo fchs32(s);
//    assert(Fibo("11") < true);
//    assert(Fibo("11") < 'c');
}
