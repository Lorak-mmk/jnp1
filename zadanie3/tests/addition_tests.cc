#include "fibo.h"
#include <cassert>
#include <iostream>
#include <random>


const int EQUAL_TESTS = 1'000'000;
const int NOT_EQUAL_TESTS = 100'000;
const int LIMIT = 10'000'000;

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,LIMIT);
    for(int i = 0; i < EQUAL_TESTS; i++){
        int a = dist(rng);
        int b = dist(rng);
        assert(Fibo(a) + Fibo(b) == Fibo(a + b));
    }
    
    for(int i = 0; i < NOT_EQUAL_TESTS; i++){
        int a = dist(rng);
        int b = dist(rng);
        int c = dist(rng);
        assert(Fibo(a) + Fibo(b) != Fibo(a + b + 1));
        assert(Fibo(a) + Fibo(b) != Fibo(c));
    }
  
    std::cout << "OK " << EQUAL_TESTS << " equality tests, " << NOT_EQUAL_TESTS << " inequality tests\n";
    return 0;
}
