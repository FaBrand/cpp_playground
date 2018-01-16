#include <iostream>
#include "state_visitor.h"
#include "statemachine.h"

void TestStatemachine()
{
    StateVisitor vis;
    Statemachine sm;

    std::cout << "> Send ActivationIntent" << std::endl;
    sm.ActivationIntent();
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
