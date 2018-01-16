#ifndef VISITABLE_STATE_H
#define VISITABLE_STATE_H

#include <boost/msm/back/state_machine.hpp>
#include "state_visitor.h"

template <class VisitorBase>
struct VisitableState
{
    typedef boost::msm::back::args<void, VisitorBase&> accept_sig;
    virtual ~VisitableState() = default;
    void accept(VisitorBase&) const
    {
        // Do nothing for the default case
    }
};

#endif /* VISITABLE_STATE_H */
