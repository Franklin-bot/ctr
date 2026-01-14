#pragma once
#include <type_traits>

namespace ctr {

// Compile-time Stack
template <class... Ts>
struct stack {};

// push()
template <class... Ts, class... As>
constexpr auto push(stack<Ts...>, As...) -> stack<As..., Ts...>;

// pop()
template <class T, class... Ts>
constexpr auto pop(stack<T, Ts...>) -> stack<Ts...>;

constexpr auto pop(stack<>) -> stack<>;

// top()
template <class T, class... Ts>
constexpr auto top(stack<T, Ts...>) -> T;

using empty_stack = ctr::stack<>;
constexpr auto top(stack<>) -> empty_stack;

// empty()
template <class>
inline constexpr bool is_empty_stack_v = false;

template <>
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
template <class... Ts>
struct deque {};

template <class... Ts, class... As>
constexpr auto push_front(deque<Ts...>, As...) -> deque<As..., Ts...>;

template <class... Ts, class... As>
constexpr auto push_back(deque<Ts...>, As...) -> deque<Ts..., As...>;

template <class T, class... Ts>
constexpr auto pop_front(deque<T, Ts...>) -> deque<Ts...>;

template <class T, class... Ts>
constexpr auto pop_back(deque<Ts..., T>) -> deque<Ts...>;

template <class T, class... Ts>
constexpr auto front(deque<T, Ts...>) -> T;

template <class T, class... Ts>
constexpr auto back(deque<Ts..., T>) -> T;

// is_deque()
template <class>
inline constexpr bool is_deque_v = false;

template <class... Ts>
inline constexpr bool is_deque_v<deque<Ts...>> = true;

template <class D>
concept Deque = is_deque_v<D>;

// helpers
template <class S>
using top_t = decltype(ctr::top(std::declval<S>()));

template <class S>
using pop_t = decltype(ctr::pop(std::declval<S>()));

template <class S, class C>
using push_t = decltype(ctr::push(std::declval<S>(), std::declval<C>()));

template <class D, class C>
using push_back_t = decltype(ctr::push_back(std::declval<D>(), std::declval<C>()));

// pair
template <class A, class B>
struct pair {
  using first_type = A;
  using second_type = B;
};

template <class>
inline constexpr bool is_pair_v = false;

template <class A, class B>
inline constexpr bool is_pair_v<pair<A, B>> = true;

template <class P>
concept Pair = is_pair_v<P>;

template <Pair P>
using first_t = typename P::first_type;

template <Pair P>
using second_t = typename P::second_type;

} // namespace ctr
