#include "concrete_observer.h"
#include <iostream>

// Which is more evil, keeping a pointer to the Subject and using RAII to unsubscribe if this object is destroyed
// Or not store the pointer to the subject and possibly have a invalid pointer dereferenced
ConcreteObserver::~ConcreteObserver()
{
    CancelSubscription();
}

void ConcreteObserver::Subscribe(Subject& sub)
{
    subject_ = &sub;
    sub.attach(this);
}

void ConcreteObserver::CancelSubscription()
{
    if (subject_)
    {
        subject_->remove(this);
    }
    subject_ = nullptr;
}

void ConcreteObserver::update(const LowLevelInput& data)
{
    std::cout << "ConcreteObserver: Got new data >> " << data.value_a_ << " - " << data.value_b_ << '\n';
}
