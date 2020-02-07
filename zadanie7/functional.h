#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <cstdio>
#include <functional>
#include <utility>

// Zarówno compose jak i lift mogą obsługiwać funkcje z wieloma argumentami zamiast 1 - bo czemu nie.
// nie dotyczy pustego compose - no bo co miałby zwracać?
// Dzięki wywoływaniu funkcji przez std::invoke zamiast () możemy np. przekazać do lift
// non-static member function lub dowolny inny Callable
// co znacznie upraszcza kod funkcji lerp i ogólnie zwiększa elastyczność compose/lift.

inline auto compose() {
    return [](auto&& arg) -> decltype(arg) { return std::forward<decltype(arg)>(arg); };
}


// Każdą funkcję kopiujemy jedynie raz, dzięki liczeniu outer przed zwróceniem lambdy.
template<typename F, typename... Fs>
inline auto compose(F&& f, Fs&& ... functions) {
    auto outer = compose(std::forward<Fs>(functions)...);
    return [f, outer](auto&& ... args)
            -> std::invoke_result_t<decltype(outer), std::invoke_result_t<F, decltype(args) ...>> {
        return std::invoke(outer, std::invoke(f, std::forward<decltype(args)>(args)...));
    };
}

template<typename H, typename... Fs>
inline auto lift(H&& h, Fs&& ... functions) {
    return [h, functions...](auto&& ... args) {
        return std::invoke(h, std::invoke(functions, std::forward<decltype(args)>(args)...)...);
    };
}

#endif  // FUNCTIONAL_H
