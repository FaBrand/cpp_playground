#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "statemachine_definition.h"

class Statemachine
{
  public:
    Statemachine();

    void ActivationIntent(bool is_allowed);

    void DeactivationIntent();

    void VisitState(StateVisitor& visitor);

  private:
    statemachine::OnOffMachine on_off_statemachine_;
};

#endif /* STATEMACHINE_H */
