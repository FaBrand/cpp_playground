#include "client.h"

void Client::SetStrategy(std::unique_ptr<Strategy>&& new_strategy)
{
    current_strategy_ = std::move(new_strategy);
}

void Client::ExecuteStrategy()
{
    if (current_strategy_)
    {
        current_strategy_->DoWork();
    }
}
