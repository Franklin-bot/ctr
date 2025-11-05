#include "nfa.hpp"

#include <stack>

namespace ctr{

NFA NFA::EpsilonNFA(){

    std::unique_ptr<State> s0 = std::make_unique<State>();
    std::unique_ptr<State> s1 = std::make_unique<State>();
    s0->AddTransition(EpsilonTransition(s1.get()));

    NFA nfa = NFA();

    nfa.SetStartState(s0.get());
    nfa.SetFinishState(s1.get());
    nfa.AddState(std::move(s0));
    nfa.AddState(std::move(s1));

    return nfa;
}

NFA NFA::SymbolNFA(char_t c){

    std::unique_ptr<State> s0 = std::make_unique<State>();
    std::unique_ptr<State> s1 = std::make_unique<State>();
    s0->AddTransition(SymbolTransition(s1.get(), c));

    NFA nfa = NFA();

    nfa.SetStartState(s0.get());
    nfa.SetFinishState(s1.get());
    nfa.AddState(std::move(s0));
    nfa.AddState(std::move(s1));

    return nfa;
}

void NFA::KleeneNFA() {

    auto s0 = std::make_unique<State>();
    auto s1 = std::make_unique<State>();

    s0->AddTransition(EpsilonTransition(start_state));
    s0->AddTransition(EpsilonTransition(s1.get()));
    finish_state->AddTransition(EpsilonTransition(start_state));
    finish_state->AddTransition(EpsilonTransition(s1.get()));

    SetStartState(s0.get());
    SetFinishState(s1.get());

    AddState(std::move(s0));
    AddState(std::move(s1));
}

void NFA::UnionNFA(NFA&& other){

    State* other_start_state = other.GetStartState();
    State* other_finish_state = other.GetFinishState();

    AddStates(std::move(other.GetStates()));

    std::unique_ptr<State> s0 = std::make_unique<State>();
    std::unique_ptr<State> s1 = std::make_unique<State>();
    s0->AddTransition(EpsilonTransition(start_state));
    s0->AddTransition(EpsilonTransition(other_start_state));
    finish_state->AddTransition(EpsilonTransition(s1.get()));
    other_finish_state->AddTransition(EpsilonTransition(s1.get()));

    SetStartState(s0.get());
    SetFinishState(s1.get());

    AddState(std::move(s0));
    AddState(std::move(s1));

}

void NFA::ConcatNFA(NFA&& other){
    State* other_start  = other.GetStartState();
    State* other_finish = other.GetFinishState();

    finish_state->AddTransition(EpsilonTransition(other_start));

    AddStates(std::move(other.GetStates()));
    SetFinishState(other_finish);
}

string_t NFA::InsertConcat(const string_t& pattern){

    if (pattern.empty()) return {};

    string_t res{pattern.front()};
    char_t prev = pattern.front();

    for (int i = 1; i < pattern.length(); i++){
        char_t curr = pattern[i];

        if ((IsAtom(prev) || IsQuantifier(prev) || IsClosedGroup(prev)) && (IsAtom(curr) || IsNewGroup(curr))){
            res += Concat.op;
        }
        res += curr;
        prev = curr;
    }
    return res;
}

string_t NFA::ShuntingYard(string_t& pattern){

    pattern = InsertConcat(pattern);

    string_t res{};
    std::stack<RegexOp> operator_stack;

    for (char_t c : pattern){

        if (std::isalnum(c)){
            res += c;
        } else if (c == NewGroup.op){
            operator_stack.push(NewGroup);
        } else if (c == CloseGroup.op){
            while (!operator_stack.empty() && operator_stack.top() != NewGroup){
                res += operator_stack.top().op;
                operator_stack.pop();
            }
            operator_stack.pop();
        } else {
            RegexOp curr_op = GetRegexOp(c);
            while (!operator_stack.empty() && operator_stack.top() != NewGroup && (operator_stack.top().precedence > curr_op.precedence || (operator_stack.top().precedence == curr_op.precedence && curr_op.left_assoc))){
                res += operator_stack.top().op;
                operator_stack.pop();
            }
            operator_stack.push(curr_op);
        }
    }

    while (!operator_stack.empty()){
        res += operator_stack.top().op;
        operator_stack.pop();
    }

    return res;

}
    
NFA NFA::NFAFromRegex(string_t& pattern){

    string_t postfix = ShuntingYard(pattern);
    std::stack<NFA> stk;

    for (char_t c : postfix){
        if (std::isalnum(c)){
            stk.push(SymbolNFA(c));
        } else {

            RegexOp regex_op = GetRegexOp(c);
            if (regex_op == Kleene){
                stk.top().KleeneNFA();
            } else if (regex_op == Concat){
                NFA& nfa2 = stk.top();
                stk.pop();
                stk.top().ConcatNFA(std::move(nfa2));
           } else {
                NFA& nfa2 = stk.top();
                stk.pop();
                stk.top().UnionNFA(std::move(nfa2));
           }
        }
    }

    NFA nfa = std::move(stk.top());
    stk.pop();
    return nfa;
}

string_t NFA::RegexFromNFA(){

    string_t res{};

}

};
