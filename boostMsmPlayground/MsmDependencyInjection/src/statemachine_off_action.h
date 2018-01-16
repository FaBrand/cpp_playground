#ifndef STATEMACHINE_OFF_ACTION_H
#define STATEMACHINE_OFF_ACTION_H

class StatemachineOffAction
{
  public:
    virtual ~StatemachineOffAction();
    virtual void ExecuteAction() = 0;
};

#endif /* STATEMACHINE_OFF_ACTION_H */
