#ifndef NOTIFICATION_HELPER_H
#define NOTIFICATION_HELPER_H

#include <memory>

#include "low_level_input.h"
#include "observer.h"

namespace detail
{

void Notify(Observer& observer, LowLevelInput const& new_data);

template <typename pointee>
void Notify(std::weak_ptr<pointee>& observer, LowLevelInput const& new_data)
{
    if (auto sh_ptr = observer.lock())
    {
        Notify(*sh_ptr, new_data);
    }
}

template <typename pointee>
void Notify(std::shared_ptr<pointee>& sh_ptr, LowLevelInput const& new_data)
{
    Notify(*sh_ptr, new_data);
}

template <typename iterable>
void Notify(iterable& collection, LowLevelInput const& new_data)
{
    for (auto& subscriber : collection)
    {
        Notify(subscriber, new_data);
    }
}

} /* detail */

#endif /* NOTIFICATION_HELPER_H */
