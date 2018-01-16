#ifndef STATEVISITOR_H
#define STATEVISITOR_H

#include <iomanip>
#include <iostream>
#include "utils/utils.h"

struct StateVisitor
{
    template <class T>
    void visit_state(T* state)
    {
        std::cout << "Visited State with type ";
        GetName(*state);
    }
};

#endif /* STATEVISITOR_H */
