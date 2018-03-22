#ifndef WORKER_FACTORY_H
#define WORKER_FACTORY_H

#include <memory>

template <typename T>
std::unique_ptr<T> MakeWorker()
{
    return std::make_unique<T>();
}

#endif  // WORKER_FACTORY_H
