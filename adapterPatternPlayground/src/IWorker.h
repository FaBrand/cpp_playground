#ifndef IWORKER_H
#define IWORKER_H

class IWorker
{
  public:
    virtual ~IWorker() = default;
    virtual void DoSomeWork() = 0;
};

#endif  // IWORKER_H
