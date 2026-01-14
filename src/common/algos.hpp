#include <cstddef>
#include <sys/wait.h>

#include "./fixed_string.hpp"
#include "./data_structures.hpp"
#include "./constants.hpp"


namespace ctr {

template<class S, class D, template<class, class...> class P, class... Args>
struct PopWhile;

template <ctr::EmptyStack S, ctr::Deque D, template<class, class...> class P, class... Args>
struct PopWhile<S, D, P, Args...>{

    using type = ctr::pair<S, D>;
};

template <ctr::Stack S, ctr::Deque D, template<class, class...> class P, class... Args>
struct PopWhile<S, D, P, Args...>{

    using top = ctr::top_t<S>;

    using type = std::conditional_t<(P<top, Args...>::value),
            typename PopWhile<ctr::pop_t<S>, ctr::push_back_t<D, top>, P, Args...>::type,
            ctr::pair<S, D>>;
};

template <ctr::Stack S, ctr::Deque D, template<class, class...> class P, class... Args>
using PopWhile_t = typename PopWhile<S, D, P, Args...>::type;


template <class T>
struct IsNotLeftParen {
    static constexpr bool value = (T::value != ctr::LEFTPAREN);
};

// pop stack until matching left parenthesis
template <ctr::Stack S, ctr::Deque D>
using PopOnRightParentheses_t = PopWhile_t<S, D, IsNotLeftParen>;

template<class T, class O>
struct IsPopOperator {
    static constexpr bool value = (T::value != ctr::LEFTPAREN) && precedence<T>() >= precedence<O>();
};

// pop stack until lower precendence operator
template <ctr::Stack S, ctr::Deque D, class O>
using PopOnOperator_t = PopWhile_t<S, D, IsPopOperator, O>;

template<class>
struct True {
    static constexpr bool value = true;
};

// pop all remaining
template <ctr::Stack S, ctr::Deque D>
using PopAll_t = PopWhile_t<S, D, True>;

template <size_t Pos, fixed_string Str, class S=ctr::stack<>, class D=ctr::deque<>>
struct ShuntingYardStep{

    static constexpr char c = Str[Pos];

};

template <size_t Pos, fixed_string Str, class S, class D>
requires (Pos >= Str.size())
struct ShuntingYardStep<Pos, Str, S, D>{
    using type = ctr::first_t<PopAll_t<S, D>>;
};



}


