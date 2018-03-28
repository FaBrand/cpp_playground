#include "concrete_observer.h"
#include <iostream>

ConcreteObserver::~ConcreteObserver()
{
    std::cout << "ConcreteObserver destroyed" << '\n';
    CancelSubscription();
}

void ConcreteObserver::Subscribe(std::shared_ptr<Subject> sub)
{
    // Using this there is a circular dependency from the subject to the observer
    // Therefore neither of them would get destroyed --> leak
    // subject_ = sub;

    sub->attach(shared_from_this());
}

void ConcreteObserver::CancelSubscription()
{
    if (subject_)
    {
        // Is never called since subject_ is
        subject_->remove(shared_from_this());
    }

    subject_ = nullptr;
}

void ConcreteObserver::update(const LowLevelInput& data)
{
    std::cout << "ConcreteObserver: Got new data >> " << data.value_a_ << " - " << data.value_b_ << '\n';
}
