#ifndef NOTIFICATION_HELPER_H
#define NOTIFICATION_HELPER_H

#include <memory>
#include "observer.h"

struct LowLevelInput;

namespace detail
{

void Notify(Observer& observer, LowLevelInput const& new_data);

template <typename pointee>
void Notify(std::weak_ptr<pointee> const& observer, LowLevelInput const& new_data);

template <typename pointee>
void Notify(std::shared_ptr<pointee> const& sh_ptr, LowLevelInput const& new_data);

template <typename iterable>
void Notify(iterable const& subscribers, LowLevelInput const& new_data);

template <typename pointee>
void Notify(std::weak_ptr<pointee> const& observer, LowLevelInput const& new_data)
{
    if (auto sh_ptr = observer.lock())
    {
        Notify(*sh_ptr, new_data);
    }
}

template <typename pointee>
void Notify(std::shared_ptr<pointee> const& sh_ptr, LowLevelInput const& new_data)
{
    Notify(*sh_ptr, new_data);
}

template <typename iterable>
void Notify(iterable const& subscribers, LowLevelInput const& new_data)
{
    for (auto const& subscriber : subscribers)
    {
        Notify(subscriber, new_data);
    }
}

} /* detail */

#endif /* NOTIFICATION_HELPER_H */
