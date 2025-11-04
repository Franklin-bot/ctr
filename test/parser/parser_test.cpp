#include "../../src/parser/nfa.hpp"







int main() {

    ctr::string_t pattern{"a*(a|b)"};
    ctr::string_t res1 = ctr::NFA::InsertConcat(pattern);
    ctr::print(res1);
    ctr::string_t res2 = ctr::NFA::ShuntingYard(res1);
    ctr::print(res2);

    return 0;


}




