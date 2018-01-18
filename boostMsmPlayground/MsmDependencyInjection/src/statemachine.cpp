#include "statemachine.h"
#include "statemachine_events.h"

Statemachine::Statemachine(StatemachineOnAction& on_action, StatemachineOffAction& off_action)
    : on_action_(on_action),
      off_action_(off_action),
      on_off_statemachine_([this]() { on_action_(); }, [this]() { off_action_(); })
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

void Statemachine::ExecuteStateAction()
{
    on_off_statemachine_.process_event(statemachine::StateAction());
}
