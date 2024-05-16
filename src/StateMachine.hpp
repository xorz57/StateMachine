#include <functional>
#include <tuple>
#include <unordered_map>

namespace xorz57 {
    struct transition_table_hash_t {
        template<class state_t, class event_t>
        std::size_t operator()(const std::pair<state_t, event_t> &p) const {
            auto hash1 = std::hash<state_t>{}(p.first);
            auto hash2 = std::hash<event_t>{}(p.second);
            return hash1 ^ hash2;
        }
    };

    struct transition_table_key_equal_t {
        template<class state_t, class event_t>
        bool operator()(const std::pair<state_t, event_t> &p1, const std::pair<state_t, event_t> &p2) const {
            return p1.first == p2.first && p1.second == p2.second;
        }
    };

    using guard_t = std::function<bool()>;
    using action_t = std::function<void()>;
    using enter_action_t = std::function<void()>;
    using leave_action_t = std::function<void()>;

    // template<typename state_t, typename event_t>
    // using transition_table_t = std::unordered_map<std::pair<state_t, event_t>, std::tuple<guard_t, action_t, state_t>, transition_table_hash_t, transition_table_key_equal_t>;

    template<typename state_t, typename event_t>
    using transition_table_t = std::vector<std::pair<std::pair<state_t, event_t>, std::tuple<guard_t, action_t, state_t>>>;

    template<typename state_t, typename event_t>
    class state_machine_t {
    public:
        state_machine_t() = default;

        state_machine_t(const state_t &state, transition_table_t<state_t, event_t> transition_table) : m_state(state), m_transition_table(std::move(transition_table)) {}

        bool handle_event(const event_t &event) {
            // const auto it = m_transition_table.find({m_state, event});
            const auto it = std::find_if(m_transition_table.begin(), m_transition_table.end(), [&](const auto &transition) {
                return transition.first.first == m_state && transition.first.second == event;
            });
            if (it != m_transition_table.end()) {
                const auto &guard = std::get<0>(it->second);
                const auto &action = std::get<1>(it->second);
                const auto &state = std::get<2>(it->second);
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
                return false;
            }
            return false;
        }

        state_t get_state() const {
            return m_state;
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
        std::unordered_map<state_t, enter_action_t> m_enter_actions;
        std::unordered_map<state_t, leave_action_t> m_leave_actions;
    };
}// namespace xorz57
