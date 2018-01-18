#include <iostream>
#include "statemachine.h"
#include "statemachine_off_action.h"
#include "statemachine_on_action.h"

class ConcreteOnAction : public StatemachineOnAction
{
  public:
    void operator()() override
    {
        std::cout << "On action" << std::endl;
    }
};

class ConcreteOffAction : public StatemachineOffAction
{
  public:
    void operator()() override
    {
        std::cout << "Off action" << std::endl;
    }
};

void TestStatemachine()
{
    bool is_activation_allowed = true;

    ConcreteOnAction on;
    ConcreteOffAction off;
    Statemachine sm{on, off};

    std::cout << "> Send ActivationIntent" << std::endl;
    sm.ActivationIntent(is_activation_allowed);
    sm.ExecuteStateAction();

    std::cout << "> Send DeactivationIntent" << std::endl;
    sm.DeactivationIntent();
    sm.ExecuteStateAction();
}

int main()
{
    TestStatemachine();
    return 0;
}
