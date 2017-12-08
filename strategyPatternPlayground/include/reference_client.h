#ifndef REFERENCE_CLIENT_H
#define REFERENCE_CLIENT_H

#include <functional>
#include "null_strategy.h"
#include "strategy.h"

class ReferenceClient
{
  public:
    template <typename T = Strategy>
    void SetStrategy(T& new_strategy)
    {
        static_assert(std::is_base_of<Strategy, T>::value, "Parameter must be derived of Strategy");
        current_strategy_ = new_strategy;
    }

    void ExecuteStrategy();

  private:
    NullStrategy default_strategy;
    std::reference_wrapper<Strategy> current_strategy_{default_strategy};
};

#endif /* REFERENCE_CLIENT_H */
