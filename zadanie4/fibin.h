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

// ====== Evaluating Language ======
template <typename Expression, typename Enviroment>
struct Eval {};

template <typename T, typename Env>
struct Eval<Lit<T>, Env> {
    using result = T;
};

template <typename A, typename B>
struct Add {
    template <typename ValueType>
    static constexpr ValueType value = A::template value<ValueType> + B::template value<ValueType>;
};

template <typename Env, typename A, typename B, typename... Args>
struct Eval<Sum<A, B, Args...>, Env> {
    using result = typename Eval<Sum<A, Sum<B, Args...>>, Env>::result;
};

template <typename Env, typename A, typename B>
struct Eval<Sum<A, B>, Env> {
    using a = typename Eval<A, Env>::result;
    using b = typename Eval<B, Env>::result;
    using result = Add<a, b>;
};

template <typename Env, typename A>
struct Eval<Inc1<A>, Env> {
    using result = typename Eval<Sum<A, Lit<Fib<1>>>, Env>::result;
};

template <typename Env, typename A>
struct Eval<Inc10<A>, Env> {
    using result = typename Eval<Sum<A, Lit<Fib<10>>>, Env>::result;
};

template <typename A, typename B>
struct Same {
    using answer = False;
};

template <typename A>
struct Same<A, A> {
    using answer = True;
};

template <typename Left, typename Right, typename Env>
struct Eval<Eq<Left, Right>, Env> {
    using left = typename Eval<Left, Env>::result;
    using right = typename Eval<Right, Env>::result;
    using result = typename Same<left, right>::answer;
};

template <uint64_t VarID, typename Value, typename Expr, typename Env>
struct Eval<Let<VarID, Value, Expr>, Env> {
    using value = typename Eval<Value, Env>::result;
    using result = typename Eval<Expr, EnvEntry<VarID, value, Env>>::result;
};

template <uint64_t VarID, typename Env>
struct Eval<Ref<VarID>, Env> {
    using result = typename EnvLookup<VarID, Env>::result;
};

template <typename Cond, typename Then, typename Else, typename Env>
struct Eval<If<Cond, Then, Else>, Env> {
    using result = typename Eval<If<typename Eval<Cond, Env>::result, Then, Else>, Env>::result;
};

template <typename Then, typename Else, typename Env>
struct Eval<If<True, Then, Else>, Env> {
    using result = typename Eval<Then, Env>::result;
};

template <typename Then, typename Else, typename Env>
struct Eval<If<False, Then, Else>, Env> {
    using result = typename Eval<Else, Env>::result;
};

template <uint64_t VarID, typename Body, typename Env>
struct Callable {
    template <typename Param>
    using result = typename Eval<Body, EnvEntry<VarID, Param, Env>>::result;
};

template <uint64_t VarID, typename Body, typename Env>
struct Eval<Lambda<VarID, Body>, Env> {
    using result = Callable<VarID, Body, Env>;
};

template <typename Body, typename Param, typename Env>
struct Eval<Invoke<Body, Param>, Env> {
    using EvaluatedParam = typename Eval<Param, Env>::result;
    using EvaluatedBody = typename Eval<Body, Env>::result;
    using result = typename EvaluatedBody::template result<EvaluatedParam>;
};
// ====== End Evaluating Language ======

// ====== Fibin class ======
template <typename ValueType>
class Fibin {
   public:
    template <typename Expr, typename X = ValueType, std::enable_if_t<std::is_integral<X>::value, int> = 0>
    static constexpr ValueType eval() {
        return Eval<Expr, EmptyEnv>::result::template value<ValueType>;
    }

    template <typename Expr, typename X = ValueType, std::enable_if_t<!std::is_integral<X>::value, int> = 0>
    static void eval() {
        std::cout << "Fibin doesn't support: " << typeid(ValueType).name() << "\n";
    }
};
// ====== End Fibin class ======

#endif
