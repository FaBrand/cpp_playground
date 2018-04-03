#include "concrete_observer.h"
#include <iostream>

#include "low_level_input.h"

void ConcreteObserver::update(const LowLevelInput& data)
{
    std::cout << "ConcreteObserver: Got new data >> " << data.value_a_ << " - " << data.value_b_ << '\n';
}
