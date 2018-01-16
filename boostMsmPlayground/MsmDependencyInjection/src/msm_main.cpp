#include <iostream>
#include "statemachine.h"

void TestStatemachine()
{
    bool is_activation_allowed = true;

    Statemachine sm;

    std::cout << "> Send ActivationIntent" << std::endl;
    sm.ActivationIntent(is_activation_allowed);

    std::cout << "> Send DeactivationIntent" << std::endl;
    sm.DeactivationIntent();
}

int main()
{
    TestStatemachine();
    return 0;
}
