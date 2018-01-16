#include "statemachine.h"
#include "statemachine_events.h"

Statemachine::Statemachine()
{
    on_off_statemachine_.start();
}

void Statemachine::ActivationIntent(bool is_allowed)
{
    on_off_statemachine_.process_event(statemachine::ActivationEvent(is_allowed));
}

void Statemachine::DeactivationIntent()
{
    on_off_statemachine_.process_event(statemachine::DeactivationEvent());
}

void Statemachine::VisitState(StateVisitor& visitor)
{
    on_off_statemachine_.visit_current_states(visitor);
}
