#include "StateMachine/StateMachine2.hpp"

#include <iostream>

enum class state {
    state0,
    state1,
    state2
};

enum class event {
    event1,
    event2
};

static std::string to_string(const state &state) {
    switch (state) {
        case state::state0:
            return "state0";
        case state::state1:
            return "state1";
        case state::state2:
            return "state2";
    }
    return "unknown";
}

namespace action {
    const auto action1 = []() { std::cout << "action1" << std::endl; };
    const auto action2 = []() { std::cout << "action2" << std::endl; };
}// namespace action

int main() {
    transition_table_t<state, event> tt{
            {{state::state0, event::event1}, {action::action1, state::state1}},
            {{state::state1, event::event2}, {action::action2, state::state2}},
            {{state::state2, event::event1}, {action::action1, state::state1}},
    };

    state_machine_t<state, event> sm(state::state0, tt);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.set_enter_action(state::state1, []() { std::cout << "enter_action1" << std::endl; });
    sm.set_leave_action(state::state1, []() { std::cout << "leave_action1" << std::endl; });

    sm.handle_event(event::event1);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::event2);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::event1);
    std::cout << to_string(sm.get_state()) << std::endl;

    return 0;
}
