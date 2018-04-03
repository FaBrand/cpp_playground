#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>

struct LowLevelInput;

class Observer : public std::enable_shared_from_this<Observer>
{
  public:
    virtual ~Observer() = default;

    virtual void update(LowLevelInput const& data) = 0;

    template <typename T>
    void Subscribe(T& subject)
    {
        subject.attach(shared_from_this());
    }
};

#endif /* OBSERVER_H */
