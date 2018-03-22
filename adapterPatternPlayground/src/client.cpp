#include "client.h"
#include <iostream>

void Client::HireWorker(std::unique_ptr<IWork>&& slave)
{
    slaves_.push_back(std::move(slave));
}

void Client::DoSomething()
{
    DelegateToWorker();
}

void Client::DelegateToWorker()
{
    if (WorkerIsPresent())
    {
        for (auto& slave : slaves_)
        {
            slave->DoSomeWork();
        }
    }
    else
    {
        std::cout << "I must hire a worker first." << '\n';
    }
}

bool Client::WorkerIsPresent() const
{
    return !slaves_.empty();
}
