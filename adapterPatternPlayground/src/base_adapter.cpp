#include "client.h"
#include "print_worker.h"
#include "translator.h"

int main()
{

    Client some_client;

    some_client.DoSomething();

    some_client.HireWorker(MakePrintWorker());

    some_client.DoSomething();

    some_client.HireWorker(std::make_unique<Translator>());

    some_client.DoSomething();

    return 0;
}
