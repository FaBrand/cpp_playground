#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <memory>
#include "observer.h"

struct LowLevelInput;

class ConcreteObserver : public Observer<ConcreteObserver>
{
  public:
    void update_impl(LowLevelInput const& data);
};

#endif /* CLIENT_H */
