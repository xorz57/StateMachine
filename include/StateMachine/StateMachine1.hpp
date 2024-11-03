/*
    MIT License

    Copyright (c) 2024 George Fotopoulos

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

#include <algorithm>
#include <functional>
#include <tuple>
#include <utility>
#include <vector>

using action_t = std::function<void()>;

template<typename state_t, typename event_t>
using transition_t = std::pair<std::pair<state_t, event_t>, std::tuple<action_t, state_t>>;

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
            const action_t &action = std::get<0>(it->second);
            const state_t &state = std::get<1>(it->second);
            m_state = state;
            action();
            return true;
        }
        return false;
    }

    void set_state(const state_t &state) {
        m_state = state;
    }

    void set_transition_table(const transition_table_t<state_t, event_t> &transition_table) {
        m_transition_table = transition_table;
    }

    state_t get_state() const {
        return m_state;
    }

    transition_table_t<state_t, event_t> get_transition_table() const {
        return m_transition_table;
    }

private:
    state_t m_state;
    transition_table_t<state_t, event_t> m_transition_table;
};
