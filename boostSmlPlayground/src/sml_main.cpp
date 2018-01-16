#include <cassert>
#include <iostream>
#include <boost/sml.hpp>
#include "utils/utils.h"

class AbstractDependency
{
  public:
    virtual ~AbstractDependency() = default;
    virtual void DoStuff() = 0;
};

class ConcreteDependency : public AbstractDependency
{
  public:
    void DoStuff() override
    {
        std::cout << "Did stuff in dependency" << std::endl;
    }
};

// Wrapper for Context for Statemachine
struct Context
{
    AbstractDependency& a;
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
const auto is_activation_allowed = [](const ActivationIntent&, Context&) { return true; };

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
                                     off + sml::on_entry<_> / [](Context& ctx) { ctx.a.DoStuff(); },
                                     on + event<DeactivationIntent> = off,
                                     on + sml::on_entry<_> / [](Context& ctx) { ctx.a.DoStuff(); });
    }
};

class Visitor
{
  public:
    template <typename TState>
    void operator()(const TState& state) const
    {
        std::cout << "Visited unspecialized state with type: ";
        GetName(state);
    }
};

template <>
void Visitor::operator()(const boost::sml::aux::string<class init>&) const
{
    std::cout << "Init action" << std::endl;
}

// Commented out to demonstrate that the original definition gets called and not a specialization
// template <>
// void Visitor::operator()(const boost::sml::aux::string<class on>&) const
// {
//     std::cout << "On action" << std::endl;
// }

template <>
void Visitor::operator()(const boost::sml::aux::string<class off>&) const
{
    std::cout << "Off action" << std::endl;
}

int main()
{
    using namespace sml;
    Visitor visitor;

    ConcreteDependency foo;
    Context ctx{foo};

    sm<OnOffMachine> sm{ctx};
    assert(sm.is(off));
    sm.visit_current_states(visitor);

    sm.process_event(ActivationIntent{});
    assert(sm.is(on));
    sm.visit_current_states(visitor);

    sm.process_event(DeactivationIntent{});
    assert(sm.is(off));
    sm.visit_current_states(visitor);

    return 0;
}
