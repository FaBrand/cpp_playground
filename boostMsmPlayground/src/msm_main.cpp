#include <iostream>
#include "state_visitor.h"
#include "statemachine.h"

void TestStatemachine()
{
    bool is_activation_allowed = true;

    StateVisitor vis;
    Statemachine sm;

    std::cout << "> Send ActivationIntent" << std::endl;
    sm.ActivationIntent(is_activation_allowed);
    sm.VisitState(vis);

    std::cout << "> Send DeactivationIntent" << std::endl;
    sm.DeactivationIntent();
    sm.VisitState(vis);
}

int main()
{
    TestStatemachine();
    return 0;
}
