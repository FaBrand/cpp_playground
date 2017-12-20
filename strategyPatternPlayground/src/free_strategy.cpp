#include "free_context.h"
#include "free_strategies.h"

int main()
{
    FreeContext context;
    context.SetStrategy([](std::string data) { return FreeStrategyA(data); });
    context.ExecuteStrategy();
    context.SetStrategy([](std::string data) { return FreeStrategyB(data); });
    context.ExecuteStrategy();
}
