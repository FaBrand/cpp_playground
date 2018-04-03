#include "notification_helper.h"

void detail::Notify(Observer& observer, LowLevelInput const& new_data)
{
    observer.update(new_data);
}
