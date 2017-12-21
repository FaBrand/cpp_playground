#ifndef FREE_CONTEXT
#define FREE_CONTEXT

#include <functional>

class FreeContext
{
  public:
    FreeContext() = default;
    void SetStrategy(std::function<void(std::string)> strategy);
    void ExecuteStrategy();

  private:
    std::function<void(std::string)> strategy_;
    std::string data_ = "myself";
};
#endif /* ifndef FREE_CONTEXT */
