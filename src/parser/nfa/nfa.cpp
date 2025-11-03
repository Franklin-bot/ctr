#include "nfa.hpp"

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

std::unique_ptr<NFA> NFA::SymbolNFA(char c){

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
    
std::unique_ptr<NFA> NFA::NFAFromPattern(std::string_view pattern){

}

};
