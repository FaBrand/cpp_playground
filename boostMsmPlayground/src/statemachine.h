#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "statemachine_definition.h"

template <class VisitorBase>
class Statemachine
{
  public:
    Statemachine()
    {
        on_off_statemachine_.start();
    }

    void ActivationIntent(bool is_allowed)
    {
        on_off_statemachine_.process_event(statemachine::ActivationEvent(is_allowed));
    }

    void DeactivationIntent()
    {
        on_off_statemachine_.process_event(statemachine::DeactivationEvent());
    }

    void VisitState(VisitorBase& visitor)
    {
        on_off_statemachine_.visit_current_states(visitor);
    }

  private:
    typename statemachine::OnOffMachine<VisitorBase>::type on_off_statemachine_;
};

#endif /* STATEMACHINE_H */
