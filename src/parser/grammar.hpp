#include <type_traits>

enum class NodeType {
    Eps,
    Lit,
    Alt,
    Concat,
    Kle
};

template <NodeType N, class... Ts>
struct Node {
    static constexpr NodeType node_type = N;
};

template <class T>
concept Expr = requires { T::node_type; } &&
  std::is_base_of_v<Node<T::node_type>, T>;

template <char C>
struct Literal : Node<NodeType::Lit>{
    static constexpr char value = C;
};

struct Epsilon : Node<NodeType::Eps> {};

template <Expr A, Expr B>
struct Alternation : Node<NodeType::Alt, A, B> {};

template <Expr A, Expr B>
struct Concatenation : Node<NodeType::Concat, A, B> {};

template <Expr A>
struct Kleene : Node<NodeType::Kle, A> {};

