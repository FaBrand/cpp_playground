#ifndef CLIENT_H
#define CLIENT_H

#include "observer.h"
#include "subject.h"

class ConcreteObserver : public Observer
{
  public:
    ConcreteObserver() = default;
    ConcreteObserver(const ConcreteObserver&) = default;
    ConcreteObserver(ConcreteObserver&&) = default;
    ConcreteObserver& operator=(const ConcreteObserver&) = default;
    ConcreteObserver& operator=(ConcreteObserver&&) = default;
    ~ConcreteObserver();

    void Subscribe(Subject& model);
    void CancelSubscription();

    void update(LowLevelInput const& data) override;

  private:
    Subject* subject_{nullptr};
};

#endif /* CLIENT_H */
