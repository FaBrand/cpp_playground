#include "client.h"
#include "print_worker.h"

int main()
{

    Client some_client{MakePrintWorker()};

    some_client.DoSomething();

    return 0;
}
