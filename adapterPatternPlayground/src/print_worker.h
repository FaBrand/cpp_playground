#ifndef PRINT_WORKER_H
#define PRINT_WORKER_H

#include <memory>
#include "IWorker.h"

std::unique_ptr<IWorker> MakePrintWorker();

class PrintWorker : public IWorker
{
  public:
    void DoSomeWork() override;
};

#endif  // PRINT_WORKER_H
