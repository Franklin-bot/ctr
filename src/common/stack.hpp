template <typename ... Ts> struct stack {};

template <typename ... Ts, typename ... As> 
constexpr auto push(stack<Ts...>, As...) -> stack<As..., Ts...>;

template <typename T, typename ... Ts> 
constexpr auto pop(stack<T, Ts...>) -> stack<Ts...>;

template <typename T, typename ... Ts> 
constexpr auto top(stack<T, Ts...>) -> T;

struct empty_stack { };

constexpr auto top(stack<>) -> empty_stack;





