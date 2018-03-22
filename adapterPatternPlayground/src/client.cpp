#include "client.h"
#include <iostream>

void Client::HireWorker(std::unique_ptr<IWork>&& slave)
{
    slave_.swap(slave);
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
    else
    {
        std::cout << "I must hire a worker first." << '\n';
    }
}

bool Client::WorkerIsPresent() const
{
    return slave_ != nullptr;
}
