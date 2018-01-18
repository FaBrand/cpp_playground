#ifndef STATEMACHINE_ON_ACTION_H
#define STATEMACHINE_ON_ACTION_H

class StatemachineOnAction
{
  public:
    virtual ~StatemachineOnAction() = default;
    virtual void operator()() = 0;
    // {
    //     std::cout << "Base" << std::endl;
    // };
};

#endif /* STATEMACHINE_ON_ACTION_H */
