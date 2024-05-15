#include "StateMachine.hpp"

#include <iostream>

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
    const auto action1 = []() { std::cout << "transition_action: starting" << std::endl; };
    const auto action2 = []() { std::cout << "transition_action: stopping" << std::endl; };

    const auto guard1 = []() { return true; };
    const auto guard2 = []() { return false; };

    xorz57::transition_table_t<state, event> tt{
            {{state::idle, event::start}, {guard1, action1, state::running}},
            {{state::running, event::stop}, {guard1, action2, state::stopped}},
            {{state::stopped, event::start}, {guard2, action1, state::running}},
    };

    xorz57::state_machine_t<state, event> sm(state::idle, tt);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::start);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::stop);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::start);
    std::cout << to_string(sm.get_state()) << std::endl;

    return 0;
}
