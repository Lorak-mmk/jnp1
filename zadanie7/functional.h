#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <utility>
#include <cstdio>
#include <functional>


auto compose() {
		return [](auto&& x) { return std::forward<decltype(x)>(x); };
}

template <typename F, typename... Fs>
auto compose(F&& f, Fs&&... functions) {
		return [f, functions...](auto&&... args){
				return compose(functions...)(f(std::forward<decltype(args)>(args)...));
		};
}


template<typename H, typename...Fs>
auto lift(H&& h, Fs&&... functions) {
		return [h, functions...](auto&&... args) {
				return h(std::invoke(functions, std::forward<decltype(args)>(args)...)...);
		};
}

#endif //FUNCTIONAL_H
