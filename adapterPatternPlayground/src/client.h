#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <vector>
#include "IWorker.h"

class Client
{
  public:
    void HireWorker(std::unique_ptr<IWorker>&& slave);
    void DoSomething();

  private:
    void DelegateToWorker();
    bool WorkerIsPresent() const;

    std::vector<std::unique_ptr<IWorker>> slaves_;
};

#endif  // CLIENT_H
