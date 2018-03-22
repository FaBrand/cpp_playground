#ifndef WORKER_FACTORY_H
#define WORKER_FACTORY_H

#include <memory>
#include "IWorker.h"

template <typename T>
auto MakeWorker() -> decltype(auto)
{
    using U = std::decay_t<T>;
    static_assert(std::is_base_of<IWorker, U>::value, "The instance you want to create is no worker");

    return std::make_unique<U>();
}

#endif  // WORKER_FACTORY_H
