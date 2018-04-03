#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <memory>
#include "observer.h"

struct LowLevelInput;

class ConcreteObserver : public Observer
{
  public:
    void update(LowLevelInput const& data) override;
};

#endif /* CLIENT_H */
