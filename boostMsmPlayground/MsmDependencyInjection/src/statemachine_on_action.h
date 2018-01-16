#ifndef STATEMACHINE_ON_ACTION_H
#define STATEMACHINE_ON_ACTION_H

class StatemachineOnAction
{
  public:
    virtual ~StatemachineOnAction() = default;
    virtual void ExecuteAction() = 0;
};

#endif /* STATEMACHINE_ON_ACTION_H */
