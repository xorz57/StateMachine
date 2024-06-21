#include "StateMachine/StateMachine4.hpp"

#include <iostream>
#include <string>

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

namespace guard {
    const auto guard1 = [](const auto &data) {
        return std::holds_alternative<int>(data) && std::get<int>(data) > 0;
    };
    const auto guard2 = [](const auto &) { return true; };
    const auto guard3 = [](const auto &data) {
        return std::holds_alternative<double>(data) && std::get<double>(data) == 3.14;
    };
}// namespace guard

namespace action {
    const auto action1 = [](const auto &data) {
        if (auto pval = std::get_if<int>(&data)) {
            std::cout << "action1 (" << *pval << ")" << std::endl;
        }
    };
    const auto action2 = [](const auto &data) {
        if (auto pval = std::get_if<double>(&data)) {
            std::cout << "action2 (" << *pval << ")" << std::endl;
        }
    };
}// namespace action

using data = std::variant<int, double>;

int main() {
    transition_table_t<state, event, data> tt{{{state::state0, event::event1}, {guard::guard1, action::action1, state::state1}},
                                              {{state::state1, event::event2}, {guard::guard2, action::action2, state::state2}},
                                              {{state::state2, event::event1}, {guard::guard3, action::action1, state::state0}}};

    state_machine_t<state, event, data> sm(state::state0, tt);

    sm.handle_event(event::event1, 1);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::event2, 3.14);
    std::cout << to_string(sm.get_state()) << std::endl;

    sm.handle_event(event::event1, 12);
    std::cout << to_string(sm.get_state()) << std::endl;

    return 0;
}