#ifndef STRATEGY_H
#define STRATEGY_H

class Strategy
{
  public:
    virtual ~Strategy() = default;
    virtual void DoWork() = 0;
};

#endif /* STRATEGY_H */
