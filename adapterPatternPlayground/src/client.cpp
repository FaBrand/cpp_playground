#include "client.h"
#include <iostream>

Client::Client(std::unique_ptr<IWork>&& slave) : slave_(std::move(slave))
{
}

void Client::DoSomething()
{
    std::cout << "Did something" << '\n';
}
