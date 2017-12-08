#ifndef NULL_STRATEGY_H
#define NULL_STRATEGY_H

#include "strategy.h"

class NullStrategy : public Strategy
{
  public:
    void DoWork() override{};
};

#endif /* NULL_STRATEGY_H */
