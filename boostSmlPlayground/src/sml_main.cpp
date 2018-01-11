//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <iostream>
#include <boost/sml.hpp>

namespace sml = boost::sml;

#define EVENT(x) \
    struct x     \
    {            \
    };

// events
EVENT(ActivationIntent)
EVENT(DeactivationIntent)

// guards
const auto is_activation_allowed = [](const ActivationIntent&) { return true; };

// actions
const auto active_entry_action = [] { std::cout << "active_entry_action" << std::endl; };
// const auto active_during_action = [] { std::cout << "active_during_action" << std::endl; };
const auto inactive_entry_action = [] { std::cout << "inactive_entry_action" << std::endl; };
// const auto inactive_during_action = [] { std::cout << "inactive_during_action" << std::endl; };

struct OnOffMachine
{
    auto operator()() const
    {
        using namespace sml;
        // clang-format off
        return make_transition_table(
          *"off state"_s + event<ActivationIntent> [is_activation_allowed]/ active_entry_action = "on state"_s,
           "on state"_s + event<DeactivationIntent> / inactive_entry_action = "off state"_s
        );
        // clang-format on
    }
};

int main()
{
    using namespace sml;

    sm<OnOffMachine> sm;
    static_assert(1 == sizeof(sm), "sizeof(sm) != 1b");
    assert(sm.is("off state"_s));

    sm.process_event(ActivationIntent{});
    assert(sm.is("on state"_s));

    sm.process_event(ActivationIntent{});
    assert(sm.is("off state"_s));

    return 0;
}
