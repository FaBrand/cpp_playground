#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include "strategy.h"

class Client
{
  public:
    void SetStrategy(std::unique_ptr<Strategy>&& new_strategy);
    void ExecuteStrategy();

  private:
    std::unique_ptr<Strategy> current_strategy_{nullptr};
};

#endif /* CLIENT_H */
