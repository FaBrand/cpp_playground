#include <cassert>
#include <iostream>
#include <string>
#include "client.h"
#include "concrete_strategy_a.h"
#include "concrete_strategy_b.h"

int main(int argc, char* argv[])
{
    assert(argc == 2);

    std::unique_ptr<Strategy> strategy{nullptr};

    if (*argv[1] == 'a')
    {
        strategy = std::make_unique<ConcreteStrategyA>(std::move(ConcreteStrategyA()));
    }
    else
    {
        strategy = std::make_unique<ConcreteStrategyB>(std::move(ConcreteStrategyB()));
    }

    Client concrete_client;

    concrete_client.SetStrategy(std::move(strategy));
    concrete_client.ExecuteStrategy();
    return 0;
}
