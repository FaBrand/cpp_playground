#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "IWork.h"
#include "foreign_print_worker.h"

class Translator : public IWork, private ForeignPrintWorker
{
  public:
    void DoSomeWork() override;
};

#endif  // TRANSLATOR_H
