#include "client.h"
#include <iostream>

Client::~Client()
{
    std::cout << "Client destroyed" << '\n';
    CancelSubscription();
}

void Client::Subscribe(std::shared_ptr<Subject> sub)
{
    // Using this there is a circular dependency from the subject to the observer
    // Therefore neither of them would get destroyed --> leak
    // subject_ = sub;

    sub->attach(shared_from_this());
}

void Client::CancelSubscription()
{
    if (subject_)
    {
        // Is never called since subject_ is
        subject_->remove(shared_from_this());
    }

    subject_ = nullptr;
}

void Client::update(const LowLevelInput& data)
{
    std::cout << "Client: Got new data >> " << data.value_a_ << " - " << data.value_b_ << '\n';
}
