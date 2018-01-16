#include "statemachine.h"
#include "statemachine_intents.h"

Statemachine::Statemachine()
{
    on_off_statemachine.start();
}

void Statemachine::ActivationIntent()
{
    on_off_statemachine.process_event(statemachine::ActivationIntent());
}

void Statemachine::DeactivationIntent()
{
    on_off_statemachine.process_event(statemachine::DeactivationIntent());
}

void Statemachine::VisitState(StateVisitor& visitor)
{
    on_off_statemachine.visit_current_states(visitor);
}
