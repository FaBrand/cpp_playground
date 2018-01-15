#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

namespace simple_state_machine
{
namespace msm = boost::msm;
namespace msmf = boost::msm::front;
namespace mpl = boost::mpl;

// ----- Events
struct ActivationIntent
{
};

struct DeactivationIntent
{
};

// ----- State machine
struct Statemachine : msmf::state_machine_def<Statemachine>
{
    // States
    struct Init : msmf::state<>
    {
        // Entry action
        template <class Event, class Fsm>
        void on_entry(Event const&, Fsm&) const
        {
            std::cout << "Init::on_entry()" << std::endl;
        }
        // Exit action
        template <class Event, class Fsm>
        void on_exit(Event const&, Fsm&) const
        {
            std::cout << "Init::on_exit()" << std::endl;
        }
    };

    struct On : msmf::state<>
    {
        // Entry action
        template <class Event, class Fsm>
        void on_entry(Event const&, Fsm&) const
        {
            std::cout << "On::on_entry()" << std::endl;
        }
    };

    struct Off : msmf::state<>
    {
        // Entry action
        template <class Event, class Fsm>
        void on_entry(Event const&, Fsm&) const
        {
            std::cout << "Off::on_entry()" << std::endl;
        }
    };

    // override no_transition to do nothing
    template <class Event, class Fsm>
    void no_transition(Event const&, Fsm&, int /* state */)
    {
        // Do nothing if an event doesn't lead to a transition instead of assert(false)
    }

    // Guards
    struct ActivationGuard
    {
        template <class Event, class Fsm>
        bool operator()(Event const&, Fsm&, Off&, On&) const
        {
            std::cout << "Activation Guard checked" << std::endl;
            return true;
        }
    };

    // Set initial state
    typedef Init initial_state;

    // Transition table
    struct transition_table : mpl::vector<
                                  // clang-format off
                                  //      Start  , Event              , Next , Action     , Guard
                                  msmf::Row<Init , msmf::none         , Off  , msmf::none , msmf::none>      ,
                                  msmf::Row<Off  , ActivationIntent   , On   , msmf::none , ActivationGuard> ,
                                  msmf::Row<On   , DeactivationIntent , Off  , msmf::none , msmf::none>>
    // clang-format on
    {
    };
};

// Pick a back-end
typedef msm::back::state_machine<Statemachine> Sm;

}  // namespace simple_state_machine

void test()
{
    simple_state_machine::Sm simple_state_machine;
    simple_state_machine.start();

    std::cout << "> Send ActivationIntent" << std::endl;
    simple_state_machine.process_event(simple_state_machine::ActivationIntent());
    std::cout << "> Send DeactivationIntent" << std::endl;
    simple_state_machine.process_event(simple_state_machine::DeactivationIntent());

    // simple_state_machine.stop();
}

int main()
{
    test();
    return 0;
}
