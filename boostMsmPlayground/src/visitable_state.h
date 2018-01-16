#ifndef VISITABLE_STATE_H
#define VISITABLE_STATE_H

#include <boost/msm/back/state_machine.hpp>
#include "state_visitor.h"

struct VisitableState
{
    typedef boost::msm::back::args<void, StateVisitor&> accept_sig;
    virtual ~VisitableState() = default;
    void accept(StateVisitor&) const
    {
        // Do nothing for the default case
    }
};

#endif /* VISITABLE_STATE_H */
