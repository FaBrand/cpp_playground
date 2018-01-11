#include <cassert>
#include <iostream>
#include <boost/sml.hpp>

class BaseDependency
{
  public:
    virtual ~BaseDependency() = default;
    virtual void DoStuff() = 0;
};

class ConcreteDependency : public BaseDependency
{
  public:
    void DoStuff() override
    {
        std::cout << "Did stuff" << std::endl;
    }
};

class OtherConcreteDependency : public BaseDependency
{
  public:
    void DoStuff() override
    {
        std::cout << "Did other stuff" << std::endl;
    }
};

// Wrapper for Context for Statemachine
struct Context
{
    BaseDependency& a;
    BaseDependency& b;
    BaseDependency& c;
    BaseDependency& d;
};

namespace sml = boost::sml;

#define EVENT(x) \
    struct x     \
    {            \
    };

// events
EVENT(ActivationIntent)
EVENT(DeactivationIntent)

// guards
const auto is_activation_allowed = [](Context& dep, const ActivationIntent&) {
    dep.a.DoStuff();
    dep.b.DoStuff();
    return true;
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

    ConcreteDependency foo;
    OtherConcreteDependency Foo;
    Context ctx{foo, Foo, foo, Foo};

    sm<OnOffMachine> sm{ctx};
    assert(sm.is(off));

    sm.process_event(ActivationIntent{});
    assert(sm.is(on));

    sm.process_event(DeactivationIntent{});
    assert(sm.is(off));

    return 0;
}
