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
const auto is_initialized = [x = 0]() mutable
{
    std::cout << "asked initialization" << std::endl;
    x++;
    return x > 2;
};

// states
using namespace sml;
auto init = "initial state"_s;
auto off = "off state"_s;
auto on = "on state"_s;

struct OnOffMachine
{
    auto operator()() const
    {
        using namespace sml;
        return make_transition_table(*init / [] { std::cout << "Left Init" << std::endl; } = off,
                                     init + sml::on_exit<_> / []() { std::cout << "Init State exit" << std::endl; },
                                     off + event<ActivationIntent>[is_activation_allowed] = on,
                                     off + sml::on_exit<_> / []() { std::cout << "Off State exit" << std::endl; },
                                     off + sml::on_entry<_> / []() { std::cout << "Off State entry" << std::endl; },
                                     on + event<DeactivationIntent> = off,
                                     on + sml::on_exit<_> / []() { std::cout << "On State exit" << std::endl; },
                                     on + sml::on_entry<_> / []() { std::cout << "On State entry" << std::endl; });
    }
};

int main()
{
    using namespace sml;

    sm<OnOffMachine> sm;
    assert(sm.is(off));

    sm.process_event(ActivationIntent{});
    assert(sm.is(on));

    sm.process_event(DeactivationIntent{});
    assert(sm.is(off));

    return 0;
}
