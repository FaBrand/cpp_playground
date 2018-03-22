#include "client.h"
#include <iostream>

Client::Client(std::unique_ptr<IWork>&& slave) : slave_(std::move(slave))
{
}

void Client::DoSomething()
{
    DelegateToWorker();
}

void Client::DelegateToWorker()
{
    if (WorkerIsPresent())
    {
        slave_->DoSomeWork();
    }
}

bool Client::WorkerIsPresent() const
{
    return slave_ != nullptr;
}
