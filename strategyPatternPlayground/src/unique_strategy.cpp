#include <cassert>
#include <memory>
#include "client.h"
#include "concrete_strategy_a.h"
#include "concrete_strategy_b.h"
#include "strategy.h"

int main(int argc, char* argv[])
{
    assert(argc == 2);

    std::unique_ptr<Strategy> strategy;

    if (*argv[1] == 'a')
    {
        strategy = std::make_unique<ConcreteStrategyA>();
    }
    else
    {
        strategy = std::make_unique<ConcreteStrategyB>();
    }

    Client concrete_client;

    concrete_client.SetStrategy(std::move(strategy));
    concrete_client.ExecuteStrategy();
    return 0;
}
