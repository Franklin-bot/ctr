#include "nfa.hpp"

#include <stack>

namespace ctr{

std::unique_ptr<NFA> NFA::EpsilonNFA(){

    std::unique_ptr<State> s0 = std::make_unique<State>();
    std::unique_ptr<State> s1 = std::make_unique<State>();
    s0->AddTransition(EpsilonTransition(s1.get()));

    std::unique_ptr<NFA> nfa = std::make_unique<NFA>();

    nfa->SetStartState(s0.get());
    nfa->SetFinishState(s1.get());
    nfa->AddState(std::move(s0));
    nfa->AddState(std::move(s1));

    return nfa;
}

std::unique_ptr<NFA> NFA::SymbolNFA(char_t c){

    std::unique_ptr<State> s0 = std::make_unique<State>();
    std::unique_ptr<State> s1 = std::make_unique<State>();
    s0->AddTransition(SymbolTransition(s1.get(), c));

    std::unique_ptr<NFA> nfa = std::make_unique<NFA>();


    nfa->AddState(std::move(s0));
    nfa->AddState(std::move(s1));
    nfa->SetStartState(s0.get());
    nfa->SetFinishState(s1.get());

    return nfa;
}

void NFA::KleeneNFA(NFA& nfa) {

    State* start = nfa.GetStartState();
    State* finish = nfa.GetFinishState();

    auto s0 = std::make_unique<State>();
    auto s1 = std::make_unique<State>();

    s0->AddTransition(EpsilonTransition(start));
    s0->AddTransition(EpsilonTransition(s1.get()));
    finish->AddTransition(EpsilonTransition(start));
    finish->AddTransition(EpsilonTransition(s1.get()));

    nfa.SetStartState(s0.get());
    nfa.SetFinishState(s1.get());

    nfa.AddState(std::move(s0));
    nfa.AddState(std::move(s1));
}

void NFA::UnionNFA(NFA& nfa_1, NFA&& nfa_2){

    State* start_1 = nfa_1.GetStartState();
    State* finish_1 = nfa_1.GetFinishState();
    State* start_2 = nfa_2.GetStartState();
    State* finish_2 = nfa_2.GetFinishState();

    nfa_1.AddStates(nfa_2.GetStates());

    std::unique_ptr<State> s0 = std::make_unique<State>();
    std::unique_ptr<State> s1 = std::make_unique<State>();
    s0->AddTransition(EpsilonTransition(start_1));
    s0->AddTransition(EpsilonTransition(start_2));
    finish_1->AddTransition(EpsilonTransition(s1.get()));
    finish_2->AddTransition(EpsilonTransition(s1.get()));

    nfa_1.SetStartState(s0.get());
    nfa_1.SetFinishState(s1.get());

    nfa_1.AddState(std::move(s0));
    nfa_1.AddState(std::move(s1));

}

void NFA::ConcatNFA(NFA& nfa_1, NFA&& nfa_2){

    State* start_1 = nfa_1.GetStartState();
    State* finish_1 = nfa_1.GetFinishState();
    State* start_2 = nfa_2.GetStartState();
    State* finish_2 = nfa_2.GetFinishState();

    nfa_1.SetFinishState(finish_2);
    finish_1->AddTransition(EpsilonTransition(start_2));
    nfa_1.AddStates(nfa_2.GetStates());

}
string_t NFA::InsertConcat(string_t pattern){

    if (pattern.empty()) return {};

    string_t res{pattern[0]};
    char_t prev = pattern[0];

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

string_t NFA::ShuntingYard(string_t pattern){

    string_t res{};
    std::stack<RegexOp> operator_stack;

    for (char_t c : pattern){

        if (isalnum(c)){
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
    
// std::unique_ptr<NFA> NFA::NFAFromPattern(string_t pattern){
//
//     string_t postfix = ShuntingYard(InsertConcat(pattern));
//
// }

};
