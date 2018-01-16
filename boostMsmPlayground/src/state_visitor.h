#ifndef STATEVISITOR_H
#define STATEVISITOR_H

#include <iomanip>
#include <iostream>
#include "utils/utils.h"

struct StateVisitor
{
    template <class T>
    void visit_state(T* state, std::string who)
    {
        std::cout << "Visited " << std::quoted(who) << " State with type ";
        GetName(*state);
    }
};

#endif /* STATEVISITOR_H */
