#include "../constants.hpp"
#include "../fixed_string.hpp"
#include "./data_structures.hpp"

#include <cstddef>

namespace ctr {

template <class S, class D, template <class, class...> class P, class... Args>
struct PopWhile;

template <
    ctr::EmptyStack S, ctr::Deque D, template <class, class...> class P, class... Args>
struct PopWhile<S, D, P, Args...> {
    using type = ctr::pair<S, D>;
};

template <ctr::Stack S, ctr::Deque D, template <class, class...> class P, class... Args>
struct PopWhile<S, D, P, Args...> {
    using top = ctr::top_t<S>;

    using type = std::conditional_t<
        (P<top, Args...>::value),
        typename PopWhile<ctr::pop_t<S>, ctr::push_back_t<D, top>, P, Args...>::type,
        ctr::pair<S, D>>;
};

template <ctr::Stack S, ctr::Deque D, template <class, class...> class P, class... Args>
using PopWhile_t = typename PopWhile<S, D, P, Args...>::type;

template <class T>
struct IsNotLeftParen {
    static constexpr bool value = (T::value != ctr::LEFTPAREN);
};

// pop stack until matching left parenthesis
template <ctr::Stack S, ctr::Deque D>
using PopOnRightParentheses_t = PopWhile_t<S, D, IsNotLeftParen>;

template <class T, class O>
struct IsPopOperator {
    static constexpr bool value =
        (T::value != ctr::LEFTPAREN) && precedence<T>() >= precedence<O>();
};

// pop stack until lower precendence operator
template <ctr::Stack S, ctr::Deque D, class O>
using PopOnOperator_t = PopWhile_t<S, D, IsPopOperator, O>;

template <class>
struct True {
    static constexpr bool value = true;
};

// pop all remaining
template <ctr::Stack S, ctr::Deque D>
using PopAll_t = PopWhile_t<S, D, True>;

struct Alnum_t {};

struct LeftParen_t {};

struct RightParen_t {};

struct Operation_t {};

template <char C, class P, class T>
struct ShuntingYardStep;

template <char C, ctr::Pair P>
struct ShuntingYardStep<C, P, Alnum_t> {
    using S = first_t<P>;
    using D = second_t<P>;

    using next = ctr::pair<S, ctr::push_back_t<D, ctr::ch<C>>>;
};

template <char C, ctr::Pair P>
struct ShuntingYardStep<C, P, LeftParen_t> {
    using S = first_t<P>;
    using D = second_t<P>;

    using next = ctr::pair<ctr::push_t<S, ctr::ch<C>>, D>;
};

template <char C, ctr::Pair P>
struct ShuntingYardStep<C, P, RightParen_t> {
    using S = first_t<P>;
    using D = second_t<P>;

    using P2 = PopOnRightParentheses_t<S, D>;
    using S2 = first_t<P2>;
    using D2 = second_t<P2>;

    using next = ctr::pair<ctr::pop_t<S2>, D2>;
};

template <char C, ctr::Pair P>
struct ShuntingYardStep<C, P, Operation_t> {
    using S = first_t<P>;
    using D = second_t<P>;

    using P2 = PopOnOperator_t<S, D, ctr::ch<C>>;
    using S2 = first_t<P2>;
    using D2 = second_t<P2>;

    using next = ctr::pair<ctr::push_t<S2, ctr::ch<C>>, D2>;
};

// general case
template <size_t Pos, fixed_string Str, class S = ctr::stack<>, class D = ctr::deque<>>
struct ShuntingYard {
    static constexpr char c = Str[Pos];

    using curr = pair<S, D>;
    using c_t = std::conditional_t<
        is_alphanumeric(c), Alnum_t,
        std::conditional_t<
            is_operation(c), Operation_t,
            std::conditional_t<(is_left_paren(c)), LeftParen_t, RightParen_t>>>;

    using next = std::conditional_t
                 < Pos<Str.size(), typename ShuntingYardStep<c, curr, c_t>::next, curr>;

    using S_next = first_t<next>;
    using D_next = second_t<next>;

    using type =
        typename std::conditional_t
        < Pos<Str.size(), ShuntingYard<Pos + 1, Str, S_next, D_next>, curr>::type;
};

// base case
template <size_t Pos, fixed_string Str, class S, class D>
requires(Pos >= Str.size())
struct ShuntingYard<Pos, Str, S, D> {
    using P_final = PopAll_t<S, D>;
    using type = second_t<P_final>;
};

} // namespace ctr
