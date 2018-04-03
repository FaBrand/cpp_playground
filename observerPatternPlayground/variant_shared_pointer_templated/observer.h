#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>

struct LowLevelInput;

template <typename Subject>
class Observer : public std::enable_shared_from_this<Observer<Subject>>
{
  public:
    virtual ~Observer() = default;

    using data_type = typename Subject::data_type;

    virtual void update(data_type const& data) = 0;

    void Subscribe(Subject& subject)
    {
        subject.attach(Observer<Subject>::shared_from_this());
    }
};

#endif /* OBSERVER_H */
