#include "free_context.h"

void FreeContext::SetStrategy(std::function<void(std::string)> strategy)
{
    strategy_ = strategy;
}

void FreeContext::ExecuteStrategy()
{
    strategy_(data_);
}
