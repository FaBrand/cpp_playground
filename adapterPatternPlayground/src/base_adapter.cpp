#include "client.h"
#include "print_worker.h"

int main()
{

    Client some_client;

    some_client.DoSomething();

    some_client.HireWorker(MakePrintWorker());

    some_client.DoSomething();

    return 0;
}
