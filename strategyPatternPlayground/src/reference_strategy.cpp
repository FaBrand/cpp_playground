#include "concrete_strategy_a.h"
#include "concrete_strategy_b.h"
#include "reference_client.h"

int main()
{
    ReferenceClient concrete_client;

    ConcreteStrategyA strategy_a;

    {
        ConcreteStrategyB strategy_b;

        concrete_client.SetStrategy(strategy_a);
        concrete_client.ExecuteStrategy();
        concrete_client.SetStrategy(strategy_b);
        concrete_client.ExecuteStrategy();
    }

    // This would crash since b is out of scope now and is destroyed
    // the reference_wrapper uses a raw-pointer internally to access its referred object
    // concrete_client.ExecuteStrategy();

    return 0;
}
