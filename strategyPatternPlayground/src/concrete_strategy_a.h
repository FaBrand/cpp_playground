#ifndef CONCRETE_STRATEGY_A_H
#define CONCRETE_STRATEGY_A_H

#include "strategy.h"

class ConcreteStrategyA : public Strategy
{
  public:
    void DoWork() override;
};

#endif /* CONCRETE_STRATEGY_A_H */
