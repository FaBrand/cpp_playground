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

    template <typename T>
    void connect(void (T::*func)(Args...), T* instance)
    {
        connect([=](Args... args) { (instance->*func)(args...); });
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


template<typename SIGNAL, typename ... SLOT>
void connect(SIGNAL& concrete_signal, SLOT&&... concrete_slot)
{
    concrete_signal.connect(std::forward<SLOT>(concrete_slot)...);
}
#endif /* SIGNAL_H */
