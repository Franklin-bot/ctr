#pragma once
#include "../common/constants.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace ctr {

class State;

class Transition {
public:
    explicit Transition(State* next_state) : next_state(next_state) {}
    virtual ~Transition() = default;                 // virtual dtor (important)
    State* GetNextState() const { return next_state; }

    virtual char_t GetSymbol() const = 0;            // make it pure virtual

protected:
    State* next_state;
};

class EpsilonTransition : public Transition {
public:
    using Transition::Transition;
    char_t GetSymbol() const override { return Epsilon; }
};

class SymbolTransition : public Transition {
public:
    SymbolTransition(State* next_state, char_t symbol)
        : Transition(next_state), symbol(symbol) {}
    char_t GetSymbol() const override { return symbol; }
private:
    char_t symbol;
};

class State {
public:
    State() = default;
    explicit State(std::vector<std::unique_ptr<Transition>> transitions)
        : transitions(std::move(transitions)) {}

    std::vector<std::unique_ptr<Transition>>& GetTransitions() { return transitions; }

    void AddTransition(std::unique_ptr<Transition> transition) {
        transitions.emplace_back(std::move(transition));
    }

private:
    std::vector<std::unique_ptr<Transition>> transitions;
};

} // namespace ctr

