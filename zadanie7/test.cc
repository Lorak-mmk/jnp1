#include <assert.h>
#include "functional.h"

int f(int a) {
		return a + 2;
}

int g(int a, int b) {
		return a + b;
}

int h(int a) {
		return a + 5;
}

int k(int z) {
		return 48 + z;
}

int l() {
		return 13;
}

auto getFunc() {
		return compose([](auto x){ return x * 2; },
						[](auto x){ return x * 4; });
}

auto getFunc2() {
		return compose([](auto x){ return x * 5; },
									 [](auto x){ return x * 10; });
}

int main() {
		assert(compose()(1) == 1);
		assert(compose(f)(2) == 4);
		assert(compose(f, f)(2) == 6);
		assert(compose(f, h)(2) == 9);
		assert(compose(g, f)(2, 3));
		assert(compose(g, h, f)(2, 3) == 12);

		assert(lift(g, f, h)(3) == 13);
		assert(lift(l)(1) == 13);

		auto func = getFunc();
		auto func2 = getFunc2();
		assert(func(8) == 64);
		assert(func2(8) == 400);

		assert(compose([](auto x) { return x + 1; },
									 [](auto x) { return x * x; })(1) == 4);

		auto iden = compose();
		assert(iden(13) == 13);
		assert(iden(std::string("abc")) == std::string("abc"));
}