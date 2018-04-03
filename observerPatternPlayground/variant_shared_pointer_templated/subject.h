#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <list>
#include <memory>

struct LowLevelInput;
#include "observer.h"

class Subject
{
  public:
    using data_type = LowLevelInput;
    void attach(std::shared_ptr<Observer<Subject>> observer);
    void operator()(data_type const& new_data);

  private:
    std::list<std::weak_ptr<Observer<Subject>>> subscribers_{};

    void RemoveDeadSubscibers();
};

#endif /* SUBJECT_H */
