#ifndef STATEMACHINE_DEFINITION_H
#define STATEMACHINE_DEFINITION_H

#include <iostream>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include "statemachine_events.h"
#include "statemachine_off_action.h"
#include "statemachine_on_action.h"

namespace statemachine
{

namespace msm = boost::msm;
namespace msmf = boost::msm::front;
namespace mpl = boost::mpl;

// ----- State machine
struct Frontend : msmf::state_machine_def<Frontend>
{
    using Callback = std::function<void()>;

    // This is not safe against possible wrong order of arguments parsed
    // The reason for this is that Boost MSM doesn't allow pure virtual interfaces so dependency injection via pure
    // virtual interfaces doesn't work
    Frontend(Callback on_action, Callback off_action) : on_action_(on_action), off_action_(off_action)
    {
    }

    // States
    struct Init : msmf::state<>
    {
        // Entry action
        template <class Event>
        void on_entry(const Event&, Frontend&) const
        {
            std::cout << "Init::on_entry()" << std::endl;
        }
    };

    struct On : msmf::state<>
    {
        // Entry action
        template <class Event>
        void on_entry(const Event&, Frontend&) const
        {
            std::cout << "On::on_entry()" << std::endl;
        }
    };

    struct Off : msmf::state<>
    {
        // Entry action
        template <class Event>
        void on_entry(const Event&, Frontend&) const
        {
            std::cout << "Off::on_entry()" << std::endl;
        }
    };

    // Guards
    struct ActivationGuard
    {
        bool operator()(const ActivationEvent& activation, Frontend&, Off&, On&) const
        {
            return activation.IsAllowed();
        }
    };

    // Actions Executed in self transitions of the respective states
    struct OffAction
    {
        void operator()(const StateAction&, Frontend& fsm, Off&, Off&) const
        {
            if (fsm.off_action_)
                fsm.off_action_();
        }
    };

    struct OnAction
    {
        void operator()(const StateAction&, Frontend& fsm, On&, On&) const
        {
            if (fsm.off_action_)
                fsm.on_action_();
        }
    };

    // This method is only defined that the internal definition doesn't call assert(false)
    template <class Event, class Fsm>
    void no_transition(const Event&, Fsm&, int /* state */)
    {
        // Do nothing if an event doesn't lead to a transition instead of assert(false)
    }

    // Set initial state
    typedef Init initial_state;

    // Transition table
    struct transition_table : mpl::vector<
                                  // clang-format off
                                  //      Start  , Event             , Next       , Action     , Guard
                                  msmf::Row<Init , msmf::none        , Off        , msmf::none , msmf::none>      ,
                                  msmf::Row<Off  , ActivationEvent   , On         , msmf::none , ActivationGuard> ,
                                  msmf::Row<Off  , StateAction       , msmf::none , OffAction  , msmf::none>      ,
                                  msmf::Row<On   , DeactivationEvent , Off        , msmf::none , msmf::none>      ,
                                  msmf::Row<On   , StateAction       , msmf::none , OnAction   , msmf::none>
                                  // clang-format on
                                  >
    {
    };

  private:
    const Callback on_action_{nullptr};
    const Callback off_action_{nullptr};
};

// Pick a back-end
typedef msm::back::state_machine<Frontend> OnOffMachine;

}  // namespace statemachine

#endif /* STATEMACHINE_DEFINITION_H */
