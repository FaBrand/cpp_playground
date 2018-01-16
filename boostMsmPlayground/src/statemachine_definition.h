#ifndef STATEMACHINE_DEFINITION_H
#define STATEMACHINE_DEFINITION_H

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include "state_visitor.h"
#include "statemachine_events.h"
#include "visitable_state.h"

namespace statemachine
{

namespace msm = boost::msm;
namespace msmf = boost::msm::front;
namespace mpl = boost::mpl;

// ----- State machine
struct OnOffStateMachineDefinition : msmf::state_machine_def<OnOffStateMachineDefinition, VisitableState>
{
    // States
    struct Init : msmf::state<VisitableState>
    {
        // Entry action
        template <class Event, class Fsm>
        void on_entry(Event const&, Fsm&) const
        {
            std::cout << "Init::on_entry()" << std::endl;
        }

        void accept(StateVisitor& visitor) const
        {
            visitor.visit_state(this, "Init");
        }
    };

    struct On : msmf::state<VisitableState>
    {
        // Entry action
        template <class Event, class Fsm>
        void on_entry(Event const&, Fsm&) const
        {
            std::cout << "On::on_entry()" << std::endl;
        }

        void accept(StateVisitor& visitor) const
        {
            visitor.visit_state(this, "On");
        }
    };

    struct Off : msmf::state<VisitableState>
    {
        // Entry action
        template <class Event, class Fsm>
        void on_entry(Event const&, Fsm&) const
        {
            std::cout << "Off::on_entry()" << std::endl;
        }
        void accept(StateVisitor& visitor) const
        {
            visitor.visit_state(this, "Off");
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
                                  msmf::Row<Off  , ActivationEvent   , On   , msmf::none , ActivationGuard> ,
                                  msmf::Row<On   , DeactivationEvent , Off  , msmf::none , msmf::none>
                                  // clang-format on
                                  >
    {
    };
};

// Pick a back-end
typedef msm::back::state_machine<OnOffStateMachineDefinition> Sm;

}  // namespace statemachine

#endif /* STATEMACHINE_DEFINITION_H */
