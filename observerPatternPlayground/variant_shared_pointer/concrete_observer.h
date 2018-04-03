#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include "observer.h"
#include "subject.h"

class ConcreteObserver : public Observer
{
  public:
    void Subscribe(Subject& model);

    void update(LowLevelInput const& data) override;
};

#endif /* CLIENT_H */
