#include "automaton.hpp"
#include <memory>
#include <unordered_map>

namespace ctr {

class NFA {
public:
  NFA() = default;
  NFA(std::vector<std::unique_ptr<State>> &&states)
      : states(std::move(states)) {}

  NFA(const NFA &other) = delete;
  NFA &operator=(const NFA &) = delete;
  NFA(NFA &&other) noexcept = default;
  NFA &operator=(NFA &&other) noexcept = default;
  ~NFA() = default;

  [[nodiscard]] static NFA EpsilonNFA();
  [[nodiscard]] static NFA SymbolNFA(char_t c);

  void KleeneNFA();
  void UnionNFA(NFA &&other);
  void ConcatNFA(NFA &&other);

  [[nodiscard]] static NFA NFAFromRegex(string_t &pattern);
  static string_t InsertConcat(const string_t &pattern);
  static string_t ShuntingYard(string_t &pattern);

  string_t RegexFromNFA() const;

  void AddState(std::unique_ptr<State> new_state) {
    states.push_back(std::move(new_state));
  }

  void AddStates(std::vector<std::unique_ptr<State>> &&new_states) {
    states.reserve(states.size() + new_states.size());
    for (auto &new_state : new_states) {
      states.push_back(std::move(new_state));
    }
    new_states.clear();
  }
  std::vector<std::unique_ptr<State>> &GetStates() { return states; }

  State *GetStartState() { return start_state; }
  State *GetFinishState() { return finish_state; }

  void SetStartState(State *state) { start_state = state; }

  void SetFinishState(State *state) { finish_state = state; }

private:
  std::unordered_map<State *, std::size_t> StateIndexMap() const;
  std::vector<std::vector<string_t>>
  InitialiseDP(std::unordered_map<State *, std::size_t> &state_index_map) const;

  std::vector<std::unique_ptr<State>> states;
  State *start_state = nullptr;
  State *finish_state = nullptr;
};

}; // namespace ctr
