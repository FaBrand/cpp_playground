#include <cassert>
#include <iostream>
#include <typeinfo>
#include <boost/sml.hpp>
#include <cxxabi.h>

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
EVENT(Active)
EVENT(Inactive)

// guards
const auto is_activation_allowed = [](const ActivationIntent&) { return true; };

// states
using namespace sml;
auto init = sml::state<class initial_state>;
auto off = sml::state<class off>;
auto on = sml::state<class on>;

struct OnOffMachine
{
    auto operator()() const
    {
        using namespace sml;
        return make_transition_table(*off + event<ActivationIntent>[is_activation_allowed] = on,
                                     off + sml::on_exit<_> / []() { std::cout << "Off State exit" << std::endl; },
                                     off +
                                         sml::on_entry<_> /
                                             [](Context& ctx) {
                                                 ctx.a.DoStuff();
                                                 std::cout << "Off State entry" << std::endl;
                                             },
                                     on + event<DeactivationIntent> = off,
                                     on + event<Active> / [] { std::cout << "ActiveAction" << std::endl; } = on,
                                     on + sml::on_exit<_> / []() { std::cout << "On State exit" << std::endl; },
                                     on + sml::on_entry<_> / []() { std::cout << "On State entry" << std::endl; });
    }
};

template <typename T>
void GetName(const T& obj)
{
    int status;
    char* realname;
    const auto& ti = typeid(obj);
    realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
    std::cout << realname << std::endl;
}

class Visitor
{
  public:
    template <typename TState>
    // Cannot be specialized to the different types of on/off/init since TState has a different namespace:
    // boost::sml::v1_1_0::aux <--> boost::sml::v1_1_0::front
    void operator()(const TState& state) const
    {
        std::cout << "Type of state: ";
        GetName(state);
        std::cout << "Is in state: " << state.c_str() << std::endl;
    }
};

template <>
void Visitor::operator()(const boost::sml::aux::string<class off>& state) const
{
    std::cout << "GOT IT" << std::endl;
}

int main()
{
    GetName(on);
    using namespace sml;
    Visitor visitor;

    ConcreteDependency foo;
    Context ctx{foo, foo, foo, foo};

    sm<OnOffMachine> sm{ctx};
    assert(sm.is(off));
    sm.visit_current_states(visitor);

    sm.process_event(ActivationIntent{});
    assert(sm.is(on));
    sm.visit_current_states(visitor);

    sm.process_event(Active{});
    assert(sm.is(on));
    sm.visit_current_states(visitor);

    sm.process_event(DeactivationIntent{});
    assert(sm.is(off));
    sm.visit_current_states(visitor);

    return 0;
}
