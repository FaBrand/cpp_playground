#include "concrete_observer.h"
#include "low_level_input.h"

void ConcreteObserver::update_impl(LowLevelInput const& data)
{
    std::cout << "ConcreteObserver: Got new data >> " << data.value_a_ << " - " << data.value_b_ << '\n';
}
