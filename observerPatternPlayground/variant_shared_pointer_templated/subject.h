#ifndef SUBJECT_H
#define SUBJECT_H

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>

#include "notification_helper.h"
#include "observer.h"

class Subject
{
  public:
    using Watcher = std::weak_ptr<Observer>;
    void attach(Watcher observer)
    {
        std::cout << "Subject: Subscribed a observer" << '\n';
        subscribers_.push_back(observer);
    }

    template <typename T>
    void operator()(T const& new_data)
    {
        std::cout << "Subject: Notifying subscribers with new data" << '\n';

        // This can be done either before or after the Subscribers have been notified
        RemoveDeadSubscibers();

        detail::Notify(subscribers_, new_data);
    }

  private:
    std::list<Watcher> subscribers_{};

    void RemoveDeadSubscibers()
    {
        subscribers_.erase(
            std::remove_if(subscribers_.begin(), subscribers_.end(), [](const auto& sub) { return sub.expired(); }),
            subscribers_.end());
    }
};

#endif /* SUBJECT_H */
