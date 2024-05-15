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
    xorz57::transition_table_t<state, event> tt{
            {{state::idle, event::start}, {state::running, []() { std::cout << "transition_action: starting" << std::endl; }}},
            {{state::running, event::stop}, {state::stopped, []() { std::cout << "transition_action: stopping" << std::endl; }}},
            {{state::stopped, event::start}, {state::running, []() { std::cout << "transition_action: starting" << std::endl; }}},
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
