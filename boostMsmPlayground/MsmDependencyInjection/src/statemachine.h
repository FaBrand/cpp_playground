#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "statemachine_definition.h"
#include "statemachine_off_action.h"
#include "statemachine_on_action.h"

class Statemachine
{
  public:
    Statemachine(StatemachineOnAction& on_action, StatemachineOffAction& off_action);

    void ActivationIntent(bool is_allowed);

    void DeactivationIntent();

    void ExecuteStateAction();

  private:
    StatemachineOnAction& on_action_;
    StatemachineOffAction& off_action_;

    statemachine::OnOffMachine on_off_statemachine_;
};

#endif /* STATEMACHINE_H */
