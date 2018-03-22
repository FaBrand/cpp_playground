#include <iostream>

#include "print_worker.h"

void PrintWorker::DoSomeWork()
{
    std::cout << "I did some concrete work" << '\n';
}

std::unique_ptr<IWork> MakePrintWorker()
{
    return std::make_unique<PrintWorker>();
}
