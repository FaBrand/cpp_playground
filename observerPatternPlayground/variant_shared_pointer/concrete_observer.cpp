#include "concrete_observer.h"
#include <iostream>

void ConcreteObserver::Subscribe(Subject& sub)
{
    sub.attach(shared_from_this());
}

void ConcreteObserver::update(const LowLevelInput& data)
{
    std::cout << "ConcreteObserver: Got new data >> " << data.value_a_ << " - " << data.value_b_ << '\n';
}
