#include "subject.h"
#include <algorithm>
#include <iostream>

void Subject::attach(std::shared_ptr<Observer> observer)
{
    std::cout << "Subject: Subscribed a observer" << '\n';
    subscriber_.push_back(observer);
}

void Subject::CleanUpSubscribers()
{
    subscriber_.erase(
        std::remove_if(subscriber_.begin(), subscriber_.end(), [](const auto& sub) { return sub.expired(); }),
        subscriber_.end());
}

void Subject::operator()(LowLevelInput const& new_data)
{
    std::cout << "Subject: Notifying subscribers with new data" << '\n';

    // This can be done either before or after the Subscribers have been notified
    CleanUpSubscribers();

    for (auto& subscriber : subscriber_)
    {
        if (auto sh_ptr = subscriber.lock())
        {
            sh_ptr->update(new_data);
        }
    }
}
