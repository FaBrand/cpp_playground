#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>

struct LowLevelInput;

template <typename ObserverImpl>
class Observer : public std::enable_shared_from_this<Observer<ObserverImpl>>
{
  public:
    virtual ~Observer() = default;

    template <typename T>
    void update(T const& data)
    {
        GetImpl().update_impl(data);
    }

    template <typename T>
    void Subscribe(T& subject)
    {
        subject.attach(Observer<ObserverImpl>::shared_from_this());
    }

  private:
    ObserverImpl& GetImpl()
    {
        return static_cast<ObserverImpl&>(*this);
    }
};

#endif /* OBSERVER_H */
