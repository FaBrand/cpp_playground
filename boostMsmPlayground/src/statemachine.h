#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "state_visitor.h"
#include "statemachine_definition.h"

class Statemachine
{
  public:
    Statemachine();

    void ActivationIntent();

    void DeactivationIntent();

    void VisitState(StateVisitor& visitor);

  private:
    StateVisitor vis;
    statemachine::Sm on_off_statemachine;
};

#endif /* STATEMACHINE_H */
