#include <iostream>
#include "fibin.h"

int main() { 
    std::cout << Fib<64>::value<uint64_t> << "\n"; 
    constexpr uint64_t x = Var("abcd");
    std::cout << x;
    
}
