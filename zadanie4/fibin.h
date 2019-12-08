#ifndef FIBIN_H
#define FIBIN_H

#include <cstdint>

// ====== Hashing variable names ======

// Returns 0 if name is wrong
constexpr uint64_t Var(const char* name) {
    uint64_t res = 0;

    std::size_t length = 0;
    while (*name) {
        char ch = *(name++);
        if (!(('1' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))) return 0;
        length++;

        if (ch >= 'A' && ch <= 'Z') ch = 'a' + (ch - 'A');

        res *= 256;
        res += ch;
    }

    if (length < 1 || 6 < length) return 0;
    return res;
}
// ====== End Hashing variable names ======

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

template <typename VarID, typename Body>
struct Lambda {};

template <typename Fun, typename Param>
struct Invoke {};

// ====== Boolean values ======
struct True {};
struct False {};
// ====== End Booleand values ======

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

// ====== Variable lookup ======
struct EmptyEnv {};

template <uint64_t VarID, typename Value, typename Env>
struct EnvEntry {};

template <uint64_t VarID, typename Env>
struct EnvLookup {};

template <uint64_t VarID>
struct EnvLookup<VarID, EmptyEnv> {};  // Variable doesn't exists

template <uint64_t VarID, typename Value, typename Env>
struct EnvLookup<VarID, EnvEntry<VarID, Value, Env>> {
    using result = Value;
};

template <uint64_t VarID, uint64_t VarID2, typename Value, typename Env>
struct EnvLookup<VarID, EnvEntry<VarID2, Value, Env>> {
    using result = EnvLookup<VarID, Env>;
};

// ====== End Variable lookup ======

template <typename ValueType, typename Expression, typename Enviroment>
struct Eval {};

template <typename ValueType, typename T, typename Env>
struct Eval<ValueType, Lit<T>, Env> {
    using result = T;
};

template <typename ValueType, typename Cond, typename Then, typename Else, typename Env>
struct Eval<ValueType, If<Cond, Then, Else>, Env> {
    using result = typename Eval<ValueType, If<typename Eval<ValueType, Cond, Env>::result, Then, Else>, Env>::result;
};

template <typename ValueType, typename Then, typename Else, typename Env>
struct Eval<ValueType, If<True, Then, Else>, Env> {
    using result = typename Eval<ValueType, Then, Env>::result;
};

template <typename ValueType, typename Then, typename Else, typename Env>
struct Eval<ValueType, If<False, Then, Else>, Env> {
    using result = typename Eval<ValueType, Else, Env>::result;
};

template <typename ValueType, uint64_t VarID, typename Env>
struct Eval<ValueType, Ref<VarID>, Env> {
    using result = typename EnvLookup<VarID, Env>::result;
};

template <typename A, typename B>
struct Same {
    using answer = False;
};

template <typename A>
struct Same<A, A> {
    using answer = True;
};

template <typename ValueType, typename Left, typename Right, typename Env>
struct Eval<ValueType, Eq<Left, Right>, Env> {
    using left = typename Eval<ValueType, Left, Env>::result;
    using right = typename Eval<ValueType, Right, Env>::result;
    using result = typename Same<left, right>::answer;
};

template <typename ValueType, uint64_t VarID, typename Value, typename Expr, typename Env>
struct Eval<ValueType, Let<VarID, Value, Expr>, Env> {
    using value = typename Eval<ValueType, Value, Env>::result;
    using result = typename Eval<ValueType, Expr, EnvEntry<VarID, value, Env>>::result;
};

template <typename ValueType, typename Env, typename A, typename B, typename... Args>
struct Eval<ValueType, Sum<A, B, Args...>, Env> {
    using result = typename Eval<ValueType, Sum<A, Sum<B, Args...>>, Env>::result;
};

template <typename A, typename B>
struct Add {
    template <typename ValueType>
    static constexpr ValueType value = A::template value<ValueType> + B::template value<ValueType>;
};

template <typename ValueType, typename Env, typename A, typename B>
struct Eval<ValueType, Sum<A, B>, Env> {
    using a = typename Eval<ValueType, A, Env>::result;
    using b = typename Eval<ValueType, B, Env>::result;
    using result = Add<a, b>;
};


template <typename ValueType, typename Env, typename A>
struct Eval<ValueType, Inc1<A>, Env> {
    using result = typename Eval<ValueType, Sum<A, Lit<Fib<1>>>, Env>::result;
};

template <typename ValueType, typename Env, typename A>
struct Eval<ValueType, Inc10<A>, Env> {
    using result = typename Eval<ValueType, Sum<A, Lit<Fib<10>>>, Env>::result;
};

template <typename ValueType>
class Fibin {
   public:
    template <typename Expr, typename X = ValueType, std::enable_if_t<std::is_integral<X>::value, int> = 0>
    static constexpr ValueType eval() {
        return Eval<ValueType, Expr, EmptyEnv>::result::template value<ValueType>;
    }

    template <typename Expr, typename X = ValueType, std::enable_if_t<!std::is_integral<X>::value, int> = 0>
    static void eval() {
        std::cout << "Fibin doesn't support: " << typeid(ValueType).name() << "\n";
    }
};

#endif
