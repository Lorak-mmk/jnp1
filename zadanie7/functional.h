#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <cstdio>
#include <functional>
#include <utility>

inline auto compose() {
    return [](auto&& x) { return std::forward<decltype(x)>(x); };
}

template <typename F, typename... Fs>
inline auto compose(F&& f, Fs&&... functions) {
    auto outer = compose(std::forward<Fs>(functions)...);
    return [f, outer](auto&&... args) {
        return std::invoke(outer, std::invoke(f, std::forward<decltype(args)>(args)...));
    };
}

template <typename H, typename... Fs>
inline auto lift(H&& h, Fs&&... functions) {
    return [h, functions...](auto&&... args) {
        return std::invoke(h, std::invoke(functions, std::forward<decltype(args)>(args)...)...);
    };
}

#endif  // FUNCTIONAL_H
