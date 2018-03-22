#ifndef IWORK_H
#define IWORK_H

class IWork
{
  public:
    virtual ~IWork() = default;
    virtual void DoSomeWork() = 0;
};

#endif  // IWORK_H
