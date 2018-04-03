#ifndef NOTIFICATION_HELPER_H
#define NOTIFICATION_HELPER_H

#include <memory>
#include "observer.h"

namespace detail
{

template <typename T, typename U>
void Notify(Observer<T>& observer, U const& new_data)
{
    observer.update(new_data);
}

template <typename pointee, typename U>
void Notify(std::weak_ptr<pointee> const& observer, U const& new_data);

template <typename pointee, typename U>
void Notify(std::shared_ptr<pointee> const& sh_ptr, U const& new_data);

template <typename iterable, typename U>
void Notify(iterable const& subscribers, U const& new_data);

template <typename pointee, typename U>
void Notify(std::weak_ptr<pointee> const& observer, U const& new_data)
{
    if (auto sh_ptr = observer.lock())
    {
        Notify(*sh_ptr, new_data);
    }
}

template <typename pointee, typename U>
void Notify(std::shared_ptr<pointee> const& sh_ptr, U const& new_data)
{
    Notify(*sh_ptr, new_data);
}

template <typename iterable, typename U>
void Notify(iterable const& subscribers, U const& new_data)
{
    for (auto const& subscriber : subscribers)
    {
        Notify(subscriber, new_data);
    }
}

} /* detail */

#endif /* NOTIFICATION_HELPER_H */
