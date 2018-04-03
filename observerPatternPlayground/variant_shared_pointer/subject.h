#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <list>
#include <memory>

struct LowLevelInput;
class Observer;

class Subject
{
  public:
    void attach(std::shared_ptr<Observer> observer);
    void operator()(LowLevelInput const& new_data);

  private:
    std::list<std::weak_ptr<Observer>> subscribers_{};

    void RemoveDeadSubscibers();
};

#endif /* SUBJECT_H */
