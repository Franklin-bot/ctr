
namespace ctr {

constexpr char LEFTPAREN = '(';
constexpr char RIGHTPAREN = ')';
constexpr char KLEENE = '*';
constexpr char ALT = '|';
constexpr char CONCAT = '&';

constexpr int KLEENE_PRECEDENCE = 3;
constexpr int CONCAT_PRECEDENCE = 2;
constexpr int ALT_PRECEDENCE = 1;

template<char C>
struct ch {
    static constexpr char value = C;
};

consteval bool is_alphanumeric(char c) {
    return (c >= 'A' && c <= 'Z') || 
            (c >= 'a' && c <='z') ||
            (c >= '0' && c <= '9');
}

consteval bool is_operation(char c){
    return c==KLEENE || c==CONCAT || c==ALT;
}

template<char C>
consteval int precedence() {
    if constexpr (C==KLEENE) return KLEENE_PRECEDENCE;
    else if constexpr (C==CONCAT) return CONCAT_PRECEDENCE;
    else if constexpr (C==ALT) return ALT_PRECEDENCE;
    else return -1;
}
    

}
