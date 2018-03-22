#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "IWorker.h"
#include "foreign_print_worker.h"

class Translator : public IWorker, private ForeignPrintWorker
{
  public:
    void DoSomeWork() override;
};

#endif  // TRANSLATOR_H
