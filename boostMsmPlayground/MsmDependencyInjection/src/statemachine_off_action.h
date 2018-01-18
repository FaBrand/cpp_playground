#ifndef STATEMACHINE_OFF_ACTION_H
#define STATEMACHINE_OFF_ACTION_H

class StatemachineOffAction
{
  public:
    virtual ~StatemachineOffAction() = default;
    virtual void operator()(){};
};

#endif /* STATEMACHINE_OFF_ACTION_H */
