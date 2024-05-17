#include <algorithm>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

template<typename state_t, typename event_t>
using transition_table_t = std::vector<std::pair<std::pair<state_t, event_t>, std::tuple<std::function<bool()>, std::function<void()>, state_t>>>;

template<typename state_t, typename event_t>
class state_machine_t {
public:
    state_machine_t() = default;

    state_machine_t(const state_t &state, transition_table_t<state_t, event_t> transition_table) : m_state(state), m_transition_table(std::move(transition_table)) {}

    bool handle_event(const event_t &event) {
        const auto it = std::find_if(m_transition_table.begin(), m_transition_table.end(), [&](const auto &p) {
            return p.first.first == m_state && p.first.second == event;
        });
        if (it != m_transition_table.end()) {
            const std::function<bool()> &guard = std::get<0>(it->second);
            const std::function<void()> &action = std::get<1>(it->second);
            const state_t &state = std::get<2>(it->second);
            if (guard() && m_state != state) {
                if (m_leave_actions.find(m_state) != m_leave_actions.end()) {
                    m_leave_actions[m_state]();
                }
                m_state = state;
                action();
                if (m_enter_actions.find(m_state) != m_enter_actions.end()) {
                    m_enter_actions[m_state]();
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

    [[nodiscard]] std::unordered_map<state_t, std::function<void()>> get_enter_actions() const {
        return m_enter_actions;
    }

    [[nodiscard]] std::unordered_map<state_t, std::function<void()>> get_leave_actions() const {
        return m_leave_actions;
    }

    void set_state(const state_t &state) {
        m_state = state;
    }

    void set_transition_table(const transition_table_t<state_t, event_t> &transition_table) {
        m_transition_table = transition_table;
    }

    void set_enter_action(const state_t &state, const std::function<void()> &enter_action) {
        m_enter_actions[state] = enter_action;
    }

    void set_leave_action(const state_t &state, const std::function<void()> &leave_action) {
        m_leave_actions[state] = leave_action;
    }

private:
    state_t m_state;
    transition_table_t<state_t, event_t> m_transition_table;
    std::unordered_map<state_t, std::function<void()>> m_enter_actions;
    std::unordered_map<state_t, std::function<void()>> m_leave_actions;
};
