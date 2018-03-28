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
    ~Subject()
    {
        std::cout << "Subject got destroyed" << '\n';
    }
    void attach(std::shared_ptr<Observer> observer);
    void remove(std::shared_ptr<Observer> observer);
    void UpdateSubjectWith(LowLevelInput const& new_data) const;

  private:
    std::list<std::shared_ptr<Observer>> subscriber_{};
};

#endif /* SUBJECT_H */
