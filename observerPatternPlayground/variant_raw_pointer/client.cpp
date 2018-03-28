#include "client.h"
#include <iostream>

// Which is more evil, keeping a pointer to the Subject and using RAII to unsubscribe if this object is destroyed
// Or not store the pointer to the subject and possibly have a invalid pointer dereferenced
Client::~Client()
{
    if (subject_)
    {
        CancelSubscription(*subject_);
    }
}

void Client::Subscribe(Subject& sub)
{
    subject_ = &sub;
    sub.attach(this);
}

void Client::CancelSubscription(Subject& sub)
{
    subject_ = nullptr;
    sub.remove(this);
}

void Client::update(const LowLevelInput& data)
{
    std::cout << "Client: Got new data >> " << data.value_a_ << " - " << data.value_b_ << '\n';
}
