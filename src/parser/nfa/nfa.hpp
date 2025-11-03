#include "automaton.hpp"
#include <memory>

namespace ctr{

class NFA {
public:

    NFA() = default;
    NFA(std::vector<std::unique_ptr<State>> states) : states(std::move(states)){}

    void AddState(std::unique_ptr<State> new_state){
        states.push_back(std::move(new_state));
    }

    void AddStates(std::vector<std::unique_ptr<State>>& new_states){
        states.reserve(states.size() + new_states.size());
        for (auto& new_state : new_states) {
            states.push_back(std::move(new_state));
        }
        new_states.clear();
    }
    std::vector<std::unique_ptr<State>>& GetStates(){
        return states;
    }

    State* GetStartState(){
        return start_state;
    }
    State* GetFinishState(){
        return finish_state;
    }

    void SetStartState(State* state){
        start_state = state;
    }

    void SetFinishState(State* state){
        finish_state = state;
    }

    static std::unique_ptr<NFA> EpsilonNFA();
    static std::unique_ptr<NFA> SymbolNFA(char c);
    static void KleeneNFA(NFA& nfa);
    static void UnionNFA(NFA& nfa_1, NFA&& nfa_2);
    static std::unique_ptr<NFA> NFAFromPattern(std::string_view pattern);

private:

    std::vector<std::unique_ptr<State>> states;
    State* start_state;
    State* finish_state;

};


};
