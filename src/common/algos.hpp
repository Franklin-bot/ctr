#include <cstddef>

#include "./fixed_string.hpp"
#include "./data_structures.hpp"
#include "./constants.hpp"

template<class S, class D>
struct PopOnRightParentheses;

template <ctr::EmptyStack S, ctr::Deque D>
struct PopOnRightParentheses<S, D>{

    using type = std::pair<S, D>;

};

template <ctr::Stack S, ctr::Deque D>
struct PopOnRightParentheses<S, D>{

    using top_t = decltype(ctr::top(S{}));

    using type = std::conditional_t<(top_t::value!=ctr::LEFTPAREN),
            typename PopOnRightParentheses<decltype(ctr::pop(S{})), decltype(ctr::push_back(D{}, top_t{}))>::type,
            std::pair<decltype(ctr::pop(S{})), D>>;
};

template <ctr::Stack S, ctr::Deque D>
using PopOnRightParentheses_t = typename PopOnRightParentheses<S, D>::type;


template<class S, class D, char O>
struct PopOnOperator;

template <ctr::EmptyStack S, ctr::Deque D, char O>
struct PopOnOperator<S, D, O>{

    using type = std::pair<S, D>;

};

template <ctr::Stack S, ctr::Deque D, char O>
struct PopOnOperator<S, D, O>{

    using top_t = decltype(ctr::top(S{}));

    using type = std::conditional_t<(is_op(top_t::value) && precedence<top_t>() >= precedence<ctr::ch<O>>()),
            typename PopOnOperator<decltype(ctr::pop(S{})), decltype(ctr::push_back(D{}, top_t{})), O>::type,
            std::pair<decltype(ctr::push(S{}, ctr::ch<O>{})), D>>;
};


template <ctr::Stack S, ctr::Deque D, char O>
using PopOnOperator_t = typename PopOnOperator<S, D, O>::type;


template <size_t Pos, fixed_string Str, class S=ctr::stack<>, class D=ctr::deque<>>
consteval fixed_string<char, Str.size()> ShuntingYard(){

    if (Pos >= Str.size()){
        return;
    }

    static constexpr char c = Str[Pos];

    using next = std::conditional_t<ctr::is_alphanumeric(c),
            ShuntingYard<Pos+1, Str, S, ctr::push_back<D, ctr::ch<c>>>,
        std::conditional_t<c=='(',
            ShuntingYard<Pos+1, Str, ctr::push<S, ctr::ch<c>>>,
        std::conditional_t<c==')',
            PopOnRightParentheses_t<S, D>,
        
        

        >;
    } 

    }






}
