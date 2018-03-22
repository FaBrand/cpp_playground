#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <vector>
#include "IWork.h"

class Client
{
  public:
    void HireWorker(std::unique_ptr<IWork>&& slave);
    void DoSomething();

  private:
    void DelegateToWorker();
    bool WorkerIsPresent() const;

    std::vector<std::unique_ptr<IWork>> slaves_;
};

#endif  // CLIENT_H
