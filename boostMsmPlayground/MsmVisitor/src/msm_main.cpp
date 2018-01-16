#include <iostream>
#include "state_visitor.h"
#include "statemachine.h"

class VisitorImpl : public StateVisitor<VisitorImpl>
{
  public:
    // CRTP implementation for the StateVisitor base class
    template <class T>
    void visit_state_impl(T*)
    {
        // This way to act upon different states technically works, however to overload for the different
        // states, either the type needs to be somehow encapsulated. Another way is to inject Function-Objects into the
        // statemachine and execute those upon self-transition. Since one call needs to be done either way i doesn't
        // matter
        // if VisitState() or e.g. ExecuteStateAction() is called.
        std::cout << "Derived Type" << std::endl;
    }
};

void TestStatemachine()
{
    bool is_activation_allowed = true;

    VisitorImpl vis;
    Statemachine<VisitorImpl> sm;

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
