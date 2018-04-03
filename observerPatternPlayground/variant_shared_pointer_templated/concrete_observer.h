#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <memory>
#include "low_level_input.h"
#include "observer.h"
#include "subject.h"

template <typename Subject>
class ConcreteObserver : public Observer<Subject>
{
  private:
    using subject_data_t = typename Observer<Subject>::data_type;

  public:
    void update(subject_data_t const& data) override
    {
        std::cout << "ConcreteObserver: Got new data >> " << data.value_a_ << " - " << data.value_b_ << '\n';
    }
};

#endif /* CLIENT_H */
