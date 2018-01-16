#ifndef STATEVISITOR_H
#define STATEVISITOR_H

#include <iomanip>
#include <iostream>
#include "utils/utils.h"

template <class VisitorImpl>
struct StateVisitor
{
    template <class T>
    void visit_state(T* state)
    {
        static_cast<VisitorImpl*>(this)->visit_state_impl(state);
    }
};

#endif /* STATEVISITOR_H */
