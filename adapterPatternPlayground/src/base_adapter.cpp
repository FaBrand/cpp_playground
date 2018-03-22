#include "client.h"
#include "print_worker.h"
#include "translator.h"
#include "worker_factory.h"

int main()
{

    Client some_client;

    some_client.DoSomething();

    some_client.HireWorker(MakeWorker<PrintWorker>());

    some_client.DoSomething();

    some_client.HireWorker(MakeWorker<Translator>());

    some_client.DoSomething();

    return 0;
}
