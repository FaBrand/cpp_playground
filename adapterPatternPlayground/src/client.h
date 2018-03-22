#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include "IWork.h"

class Client
{
  public:
    explicit Client(std::unique_ptr<IWork>&& slave);
    void DoSomething();

  private:
    void DelegateToWorker();
    bool WorkerIsPresent() const;

    std::unique_ptr<IWork> slave_{nullptr};
};

#endif  // CLIENT_H
