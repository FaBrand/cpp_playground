#ifndef SUBJECT_H
#define SUBJECT_H

#include <list>
#include "low_level_input.h"
#include "observer.h"

class Subject
{
  public:
    void attach(Observer* observer);
    void remove(Observer* observer);
    void UpdateSubjectWith(LowLevelInput const& new_data) const;

  private:
    std::list<Observer*> subscriber_{};
};

#endif /* SUBJECT_H */
