#ifndef CONCRETE_STRATEGY_B_H
#define CONCRETE_STRATEGY_B_H

#include "strategy.h"

class ConcreteStrategyB : public Strategy
{
  public:
    void DoWork() override;
};

#endif /* CONCRETE_STRATEGY_B_H */
