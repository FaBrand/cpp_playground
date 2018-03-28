#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>
#include "low_level_input.h"

class Observer : public std::enable_shared_from_this<Observer>
{
  public:
    virtual ~Observer() = default;
    virtual void update(LowLevelInput const& data) = 0;
};

#endif /* OBSERVER_H */
