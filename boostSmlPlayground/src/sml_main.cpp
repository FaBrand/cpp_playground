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
EVENT(release)
EVENT(ack)
EVENT(fin)
EVENT(timeout)

// guards
const auto is_ack_valid = [](const ack&) { return true; };
const auto is_fin_valid = [](const fin&) { return true; };

// actions
const auto send_fin = [] { std::cout << "Send fin" << std::endl; };
const auto send_ack = [] { std::cout << "Send ack" << std::endl; };

struct hello_world
{
    auto operator()() const
    {
        using namespace sml;
        // clang-format off
        return make_transition_table(
          *"established"_s + event<release> / send_fin = "fin wait 1"_s,
           "fin wait 1"_s + event<ack> [ is_ack_valid ] = "fin wait 2"_s,
           "fin wait 2"_s + event<fin> [ is_fin_valid ] / send_ack = "timed wait"_s,
           "timed wait"_s + event<timeout> / send_ack = X
        );
        // clang-format on
    }
};

int main()
{
    using namespace sml;

    sm<hello_world> sm;
    static_assert(1 == sizeof(sm), "sizeof(sm) != 1b");
    assert(sm.is("established"_s));

    sm.process_event(release{});
    assert(sm.is("fin wait 1"_s));

    sm.process_event(ack{});
    assert(sm.is("fin wait 2"_s));

    sm.process_event(fin{});
    assert(sm.is("timed wait"_s));

    sm.process_event(timeout{});
    assert(sm.is(X));  // released

    return 0;
}
