#include <algorithm>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using guard_t = std::function<bool()>;
using action_t = std::function<void()>;
using enter_action_t = std::function<void()>;
using leave_action_t = std::function<void()>;

template<typename state_t>
using enter_actions_t = std::unordered_map<state_t, enter_action_t>;

template<typename state_t>
using leave_actions_t = std::unordered_map<state_t, leave_action_t>;

template<typename state_t, typename event_t>
using transition_t = std::pair<std::pair<state_t, event_t>, std::tuple<guard_t, action_t, state_t>>;

template<typename state_t, typename event_t>
using transition_table_t = std::vector<transition_t<state_t, event_t>>;

template<typename state_t, typename event_t>
class state_machine_t {
public:
    state_machine_t() = default;

    state_machine_t(const state_t &state, transition_table_t<state_t, event_t> transition_table) : m_state(state), m_transition_table(std::move(transition_table)) {}

    bool handle_event(const event_t &event) {
        const auto it = std::find_if(m_transition_table.begin(), m_transition_table.end(), [&](const transition_t<state_t, event_t> &transition) {
            return transition.first.first == m_state && transition.first.second == event;
        });
        if (it != m_transition_table.end()) {
            const guard_t &guard = std::get<0>(it->second);
            const action_t &action = std::get<1>(it->second);
            const state_t &state = std::get<2>(it->second);
            if (guard()) {
                auto it1 = m_leave_actions.find(m_state);
                if (it1 != m_leave_actions.end()) {
                    it1->second();
                }
                m_state = state;
                action();
                auto it2 = m_enter_actions.find(m_state);
                if (it2 != m_enter_actions.end()) {
                    it2->second();
                }
                return true;
            }
            return true;
        }
        return false;
    }

    [[nodiscard]] state_t get_state() const {
        return m_state;
    }

    [[nodiscard]] transition_table_t<state_t, event_t> get_transition_table() const {
        return m_transition_table;
    }

    [[nodiscard]] enter_actions_t<state_t> get_enter_actions() const {
        return m_enter_actions;
    }

    [[nodiscard]] leave_actions_t<state_t> get_leave_actions() const {
        return m_leave_actions;
    }

    void set_state(const state_t &state) {
        m_state = state;
    }

    void set_transition_table(const transition_table_t<state_t, event_t> &transition_table) {
        m_transition_table = transition_table;
    }

    void set_enter_action(const state_t &state, const enter_action_t &enter_action) {
        m_enter_actions[state] = enter_action;
    }

    void set_leave_action(const state_t &state, const leave_action_t &leave_action) {
        m_leave_actions[state] = leave_action;
    }

private:
    state_t m_state;
    transition_table_t<state_t, event_t> m_transition_table;
    enter_actions_t<state_t> m_enter_actions;
    leave_actions_t<state_t> m_leave_actions;
};
