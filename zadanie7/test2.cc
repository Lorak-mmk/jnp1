#include <cassert>
#include "functional.h"


int main() {
    auto xd = [](int &a) -> int & {
        return a;
    };

    int cos = 1;
    int &a = compose()(cos);
    int &b = compose(xd)(cos);
    int &c = compose(xd, xd)(cos);
    a = 123;
    assert(cos == 123);
    b = 2137;
    assert(cos == 2137);
    c = 1337;
    assert(cos == 1337);

    auto xd2 = [](int a) -> int { return 2 * a; };
    a = compose()(xd2(10));
    assert(cos == 20);

    assert(compose([](auto x) { return x + 1; }, [](auto x) { return x * x; })(1) == 4);

}