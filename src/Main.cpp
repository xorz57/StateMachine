#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

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

struct transition_table_hash {
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

struct transition_table_key_equal {
    template<class T1, class T2>
    bool operator()(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2) const {
        return p1.first == p2.first && p1.second == p2.second;
    }
};

using transition_table = std::unordered_map<std::pair<state, event>, std::pair<state, std::function<void()>>, transition_table_hash, transition_table_key_equal>;

class state_machine {
public:
    explicit state_machine(const state &state) : m_state(state) {
        std::cout << "state: " << to_string(m_state) << std::endl;
    }

    state_machine(const state &state, transition_table transition_table) : m_state(state), m_transition_table(std::move(transition_table)) {
        std::cout << "state: " << to_string(m_state) << std::endl;
    }

    void handle_event(const event &event) {
        auto it = m_transition_table.find({m_state, event});
        if (it != m_transition_table.end()) {
            auto state = it->second.first;
            auto transition_action = it->second.second;
            if (m_state != state) {
                if (leave_actions.find(m_state) != leave_actions.end()) {
                    leave_actions[m_state]();
                }
                m_state = state;
                transition_action();
                if (enter_actions.find(m_state) != enter_actions.end()) {
                    enter_actions[m_state]();
                }
            }
        } else {
            std::cout << "invalid transition! state: " << to_string(m_state) << " event: " << to_string(event) << std::endl;
        }
    }

    void set_state(const state &state) {
        m_state = state;
    }

    void set_transition_table(const transition_table &transition_table) {
        m_transition_table = transition_table;
    }

    void set_enter_action(const state &state, const std::function<void()> &enter_action) {
        enter_actions[state] = enter_action;
    }

    void set_leave_action(const state &state, const std::function<void()> &leave_action) {
        leave_actions[state] = leave_action;
    }

private:
    state m_state;
    transition_table m_transition_table;
    std::unordered_map<state, std::function<void()>> enter_actions;
    std::unordered_map<state, std::function<void()>> leave_actions;
};

int main() {
    state s = state::idle;
    transition_table tt{
            {{state::idle, event::start}, {state::running, []() { std::cout << "transition_action: starting" << std::endl; }}},
            {{state::running, event::stop}, {state::stopped, []() { std::cout << "transition_action: stopping" << std::endl; }}},
            {{state::stopped, event::start}, {state::running, []() { std::cout << "transition_action: starting" << std::endl; }}},
    };

    state_machine sm(s, tt);

    // sm.set_enter_action(state::idle, []() { std::cout << "enter_action: idle" << std::endl; });
    // sm.set_enter_action(state::running, []() { std::cout << "enter_action: running" << std::endl; });
    // sm.set_enter_action(state::stopped, []() { std::cout << "enter_action: stopped" << std::endl; });

    // sm.set_leave_action(state::idle, []() { std::cout << "leave_action: idle" << std::endl; });
    // sm.set_leave_action(state::running, []() { std::cout << "leave_action: running" << std::endl; });
    // sm.set_leave_action(state::stopped, []() { std::cout << "leave_action: stopped" << std::endl; });

    sm.handle_event(event::start);
    sm.handle_event(event::stop);
    sm.handle_event(event::start);

    return 0;
}