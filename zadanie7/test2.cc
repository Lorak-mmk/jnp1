#include <cassert>
#include "functional.h"


int main() {
    int cos = 1;
    int &a = compose()(cos);
    a = 123;
    assert(cos == 123);
}