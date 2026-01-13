#include <type_traits>

namespace ctr {

    // Compile-time Stack
    template <class ... Ts> struct stack {};

    // push()
    template <class ... Ts, class ... As> 
    constexpr auto push(stack<Ts...>, As...) -> stack<As..., Ts...>;

    // pop()
    template <class T, class ... Ts> 
    constexpr auto pop(stack<T, Ts...>) -> stack<Ts...>;

    // top()
    template <class T, class ... Ts> 
    constexpr auto top(stack<T, Ts...>) -> T;

    using empty_stack = ctr::stack<>;
    constexpr auto top(stack<>) -> empty_stack;

    // empty()
    template<class>
    inline constexpr bool is_empty_stack_v = false;

    template<>
    inline constexpr bool is_empty_stack_v<stack<>> = true;

    // is_stack()
    template <class>
    inline constexpr bool is_stack_v = false;

    template <class... Ts>
    inline constexpr bool is_stack_v<stack<Ts...>> = true;

    template <class S>
    concept Stack = is_stack_v<S>;

    template <class S>
    concept EmptyStack = is_stack_v<S> && is_empty_stack_v<S>;

    // Compile-time Deque
    template <class ... Ts> struct deque {};

    template <class ... Ts, class ... As>
    constexpr auto push_front(deque<Ts...>, As...) -> deque<As..., Ts...>;

    template <class ... Ts, class ... As>
    constexpr auto push_back(deque<Ts...>, As...) -> deque<Ts..., As...>;

    template <class T, class ... Ts>
    constexpr auto pop_front(deque<T, Ts...>) -> deque<Ts...>;

    template <class T, class ... Ts>
    constexpr auto pop_back(deque<Ts..., T>) -> deque<Ts...>;

    template <class T, class ... Ts>
    constexpr auto front(deque<T, Ts...>) -> T;

    template <class T, class ... Ts>
    constexpr auto back(deque<Ts..., T>) -> T;

    // is_deque()
    template <class>
    inline constexpr bool is_deque_v = false;

    template <class... Ts>
    inline constexpr bool is_deque_v<deque<Ts...>> = true;

    template <class S>
    concept Deque = is_deque_v<S>;
}
