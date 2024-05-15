#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace xorz57 {
    struct transition_table_hash {
        template<class state_t, class event_t>
        std::size_t operator()(const std::pair<state_t, event_t> &p) const {
            auto hash1 = std::hash<state_t>{}(p.first);
            auto hash2 = std::hash<event_t>{}(p.second);
            return hash1 ^ hash2;
        }
    };

    struct transition_table_key_equal {
        template<class state_t, class event_t>
        bool operator()(const std::pair<state_t, event_t> &p1, const std::pair<state_t, event_t> &p2) const {
            return p1.first == p2.first && p1.second == p2.second;
        }
    };

    template<typename state_t, typename event_t>
    using transition_table = std::unordered_map<std::pair<state_t, event_t>, std::pair<state_t, std::function<void()>>, transition_table_hash, transition_table_key_equal>;

    template<typename state_t, typename event_t>
    class state_machine {
    public:
        explicit state_machine(const state_t &state) : m_state(state) {}

        state_machine(const state_t &state, transition_table<state_t, event_t> transition_table) : m_state(state), m_transition_table(std::move(transition_table)) {}

        bool handle_event(const event_t &event) {
            auto it = m_transition_table.find({m_state, event});
            if (it != m_transition_table.end()) {
                auto state = it->second.first;
                auto transition_action = it->second.second;
                if (m_state != state) {
                    if (m_leave_actions.find(m_state) != m_leave_actions.end()) {
                        m_leave_actions[m_state]();
                    }
                    m_state = state;
                    transition_action();
                    if (m_enter_actions.find(m_state) != m_enter_actions.end()) {
                        m_enter_actions[m_state]();
                    }
                }
                return true;
            } else {
                return false;
            }
        }

        state_t get_state() const {
            return m_state;
        }

        void set_state(const state_t &state) {
            m_state = state;
        }

        void set_transition_table(const transition_table<state_t, event_t> &transition_table) {
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
        transition_table<state_t, event_t> m_transition_table;
        std::unordered_map<state_t, std::function<void()>> m_enter_actions;
        std::unordered_map<state_t, std::function<void()>> m_leave_actions;
    };
}// namespace xorz57

enum class state {
    idle,
    running,
    stopped
};

enum class event {
    start,
    stop
};

static std::string to_string(const state &state) {
    switch (state) {
        case state::idle:
            return "idle";
        case state::running:
            return "running";
        case state::stopped:
            return "stopped";
    }
    return "unknown";
}

static std::string to_string(const event &event) {
    switch (event) {
        case event::start:
            return "start";
        case event::stop:
            return "stop";
    }
    return "unknown";
}

int main() {
    state s = state::idle;

    xorz57::transition_table<state, event> tt{
            {{state::idle, event::start}, {state::running, []() { std::cout << "transition_action: starting" << std::endl; }}},
            {{state::running, event::stop}, {state::stopped, []() { std::cout << "transition_action: stopping" << std::endl; }}},
            {{state::stopped, event::start}, {state::running, []() { std::cout << "transition_action: starting" << std::endl; }}},
    };

    xorz57::state_machine<state, event> sm(s, tt);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::start);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::stop);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::start);
    std::cout << to_string(sm.get_state()) << std::endl;

    return 0;
}
