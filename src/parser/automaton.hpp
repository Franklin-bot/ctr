#pragma once
#include "../common/constants.hpp"

#include <vector>

namespace ctr {

    class State;

    class Transition {
    public:

        explicit Transition(State* next_state) : next_state(next_state){}

        State* GetNextState() const{
            return next_state;
        }

    protected:
        State* next_state;

    };



    class EpsilonTransition : public Transition {
    public:
        using Transition::Transition;

        bool operator== (const EpsilonTransition& other){
            return next_state == other.next_state;
        }
    };



    class SymbolTransition : public Transition {
    public:

        SymbolTransition(State* next_state, char_t symbol) : Transition(next_state), symbol(symbol) {};

        char GetSymbol() const {
            return symbol;
        }
        bool operator== (const SymbolTransition& other){
            return next_state == other.next_state && symbol == other.symbol;
        }

    private:
        char_t symbol;
    };




    class State {
    public:

        State() = default;
        State(std::vector<Transition> transitions) : transitions(std::move(transitions)){}

        std::vector<Transition>& GetTransitions() {
            return transitions;
        }

        void AddTransition(Transition transition) {
            transitions.push_back(transition);
        }

    private:
        std::vector<Transition> transitions;

    };
};
