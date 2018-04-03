#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <list>
#include <memory>
#include "low_level_input.h"
#include "observer.h"

class Subject
{
  public:
    void attach(std::shared_ptr<Observer> observer);
    void operator()(LowLevelInput const& new_data);

  private:
    std::list<std::weak_ptr<Observer>> subscriber_{};

    void CleanUpSubscribers();
};

#endif /* SUBJECT_H */
