#include "subject.h"
#include <algorithm>
#include <iostream>
#include "notification_helper.h"

void Subject::attach(std::shared_ptr<Observer> observer)
{
    std::cout << "Subject: Subscribed a observer" << '\n';
    subscribers_.push_back(observer);
}

void Subject::RemoveDeadSubscibers()
{
    subscribers_.erase(
        std::remove_if(subscribers_.begin(), subscribers_.end(), [](const auto& sub) { return sub.expired(); }),
        subscribers_.end());
}

void Subject::operator()(LowLevelInput const& new_data)
{
    std::cout << "Subject: Notifying subscribers with new data" << '\n';

    // This can be done either before or after the Subscribers have been notified
    RemoveDeadSubscibers();

    detail::Notify(subscribers_, new_data);
}
