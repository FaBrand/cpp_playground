#include "statemachine.h"
#include "statemachine_events.h"

Statemachine::Statemachine()
{
    on_off_statemachine.start();
}

void Statemachine::ActivationIntent()
{
    on_off_statemachine.process_event(statemachine::ActivationEvent());
}

void Statemachine::DeactivationIntent()
{
    on_off_statemachine.process_event(statemachine::DeactivationEvent());
}

void Statemachine::VisitState(StateVisitor& visitor)
{
    on_off_statemachine.visit_current_states(visitor);
}
