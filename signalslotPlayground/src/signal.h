#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>
#include <list>

template <typename... Args>
class Signal
{
  public:
    using callable_t = void (*)(Args...);

    void connect(std::function<void(Args...)> const& funclike)
    {
        slots_.push_back(funclike);
    }

    void emit(const Args&... args) const
    {
        for (auto const& fun : slots_)
        {
            fun(args...);
        }
    }

  private:
    std::list<std::function<void(Args...)>> slots_;
};

#endif /* SIGNAL_H */
