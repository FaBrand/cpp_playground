#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
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

    void Subscribe(std::shared_ptr<Subject> model);
    void CancelSubscription();

    void update(LowLevelInput const& data) override;

  private:
    std::shared_ptr<Subject> subject_{nullptr};
};

#endif /* CLIENT_H */
