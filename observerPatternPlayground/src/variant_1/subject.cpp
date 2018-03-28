#include "subject.h"
#include <iostream>

void Subject::attach(std::shared_ptr<Observer> observer)
{
    std::cout << "Subject: Subscribed a observer" << '\n';
    subscriber_.push_back(observer);
}

void Subject::remove(std::shared_ptr<Observer> observer)
{
    std::cout << "Subject: Unsubscribed a observer" << '\n';
    subscriber_.remove(observer);
}

void Subject::UpdateSubjectWith(LowLevelInput const& new_data) const
{
    std::cout << "Subject: Notifying subscribers with new data" << '\n';
    for (auto& obs : subscriber_)
    {
        obs->update(new_data);
    }
}
