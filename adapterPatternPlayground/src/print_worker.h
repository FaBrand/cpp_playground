#ifndef PRINT_WORKER_H
#define PRINT_WORKER_H

#include <memory>
#include "IWork.h"

std::unique_ptr<IWork> MakePrintWorker();

class PrintWorker : public IWork
{
  public:
    void DoSomeWork() override;
};

#endif  // PRINT_WORKER_H
