#ifndef FIBIN_H
#define FIBIN_H

#include <cstdint>
#include <iostream>
#include <type_traits>
#include <typeinfo>

// ====== Hashing variable names ======

// Returns 0 if name is wrong
constexpr uint64_t Var(const char* name) {
    uint64_t result = 0;

    std::size_t length = 0;
    while (*name) {
        char ch = *(name++);
        if (!(('1' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z')))
            throw std::invalid_argument("invalid character");
        length++;

        if (ch >= 'A' && ch <= 'Z') ch = 'a' + (ch - 'A');

        result *= 256;
        result += ch;
    }

    if (length < 1 || 6 < length) throw std::invalid_argument("invalid name length");

    return result;
}
// ====== End Hashing variable names ======

// ====== Language constructs ======
template <typename T>
struct Lit {};

template <typename T, typename... Args>
struct Sum {};

template <typename Arg>
struct Inc1 {};

template <typename Arg>
struct Inc10 {};

template <typename Left, typename Right>
struct Eq {};

template <uint64_t VarID, typename Value, typename Expr>
struct Let {};

template <uint64_t Value>
struct Ref {};

template <typename Condition, typename Then, typename Else>
struct If {};

template <uint64_t VarID, typename Body>
struct Lambda {};

template <typename Fun, typename Param>
struct Invoke {};

struct True {};

struct False {};
// ====== End Language constructs ======

// ====== Fib<N> values ======
template <uint64_t N>
struct Fib {
    template <typename ValueType>
    static constexpr ValueType value = Fib<N - 1>::template value<ValueType> + Fib<N - 2>::template value<ValueType>;
};

template <>
struct Fib<0> {
    template <typename ValueType>
    static constexpr ValueType value = ValueType(0);
};

template <>
struct Fib<1> {
    template <typename ValueType>
    static constexpr ValueType value = ValueType(1);
};
// ====== End Fib<N> values ======

namespace impl {

// ====== Variable lookup ======
struct EmptyEnv {};

template <uint64_t VarID, typename Value, typename Env>
struct EnvEntry {};

template <uint64_t VarID, typename Env>
struct EnvLookup {};

template <uint64_t VarID>
struct EnvLookup<VarID, EmptyEnv> {};

template <uint64_t VarID, typename Value, typename Env>
struct EnvLookup<VarID, EnvEntry<VarID, Value, Env>> {
    using result = Value;
};

template <uint64_t VarID, uint64_t VarID2, typename Value, typename Env>
struct EnvLookup<VarID, EnvEntry<VarID2, Value, Env>> {
    using result = typename EnvLookup<VarID, Env>::result;
};

// ====== End Variable lookup ======

// ====== Evaluating Language ======
template <typename Expression, typename Enviroment, typename ValueType>
struct Eval {};

template <typename T, typename Env, typename V>
struct Eval<Lit<T>, Env, V> {
    using result = T;
};

template <typename A, typename B>
struct Add {
    template <typename ValueType>
    static constexpr ValueType value = A::template value<ValueType> + B::template value<ValueType>;
};

template <typename Env, typename V, typename A, typename B, typename... Args>
struct Eval<Sum<A, B, Args...>, Env, V> {
    using result = typename Eval<Sum<A, Sum<B, Args...>>, Env, V>::result;
};

template <typename Env, typename V, typename A, typename B>
struct Eval<Sum<A, B>, Env, V> {
    using a = typename Eval<A, Env, V>::result;
    using b = typename Eval<B, Env, V>::result;
    using result = Add<a, b>;
};

template <typename Env, typename V, typename A>
struct Eval<Inc1<A>, Env, V> {
    using result = typename Eval<Sum<A, Lit<Fib<1>>>, Env, V>::result;
};

template <typename Env, typename V, typename A>
struct Eval<Inc10<A>, Env, V> {
    using result = typename Eval<Sum<A, Lit<Fib<10>>>, Env, V>::result;
};

template <bool B>
struct Boolean {};

template <>
struct Boolean<true> {
    using result = True;
};

template <>
struct Boolean<false> {
    using result = False;
};

template <typename Left, typename Right, typename Env, typename V>
struct Eval<Eq<Left, Right>, Env, V> {
    static constexpr V left = Eval<Left, Env, V>::result::template value<V>;
    static constexpr V right = Eval<Right, Env, V>::result::template value<V>;
    using result = typename Boolean<left == right>::result;
};

template <uint64_t VarID, typename Value, typename Expr, typename Env, typename V>
struct Eval<Let<VarID, Value, Expr>, Env, V> {
    using VarValue = typename Eval<Value, Env, V>::result;
    using result = typename Eval<Expr, EnvEntry<VarID, VarValue, Env>, V>::result;
};

template <uint64_t VarID, typename Env, typename V>
struct Eval<Ref<VarID>, Env, V> {
    using result = typename EnvLookup<VarID, Env>::result;
};

template <typename Cond, typename Then, typename Else, typename Env, typename V>
struct Eval<If<Cond, Then, Else>, Env, V> {
    using result = typename Eval<If<typename Eval<Cond, Env, V>::result, Then, Else>, Env, V>::result;
};

template <typename Then, typename Else, typename Env, typename V>
struct Eval<If<True, Then, Else>, Env, V> {
    using result = typename Eval<Then, Env, V>::result;
};

template <typename Then, typename Else, typename Env, typename V>
struct Eval<If<False, Then, Else>, Env, V> {
    using result = typename Eval<Else, Env, V>::result;
};

template <uint64_t VarID, typename Body, typename Env, typename V>
struct Callable {
    template <typename Param>
    using result = typename Eval<Body, EnvEntry<VarID, Param, Env>, V>::result;
};

template <uint64_t VarID, typename Body, typename Env, typename V>
struct Eval<Lambda<VarID, Body>, Env, V> {
    using result = Callable<VarID, Body, Env, V>;
};

template <typename Body, typename Param, typename Env, typename V>
struct Eval<Invoke<Body, Param>, Env, V> {
    using EvaluatedParam = typename Eval<Param, Env, V>::result;
    using EvaluatedBody = typename Eval<Body, Env, V>::result;
    using result = typename EvaluatedBody::template result<EvaluatedParam>;
};
// ====== End Evaluating Language ======

} // namespace impl

// ====== Fibin class ======
template <typename ValueType>
class Fibin {
   public:
    template <typename Expr, typename X = ValueType, std::enable_if_t<std::is_integral<X>::value, int> = 0>
    static constexpr ValueType eval() {
        return impl::Eval<Expr, impl::EmptyEnv, ValueType>::result::template value<ValueType>;
    }

    template <typename Expr, typename X = ValueType, std::enable_if_t<!std::is_integral<X>::value, int> = 0>
    static void eval() {
        std::cout << "Fibin doesn't support: " << typeid(ValueType).name() << "\n";
    }
};
// ====== End Fibin class ======

#endif
