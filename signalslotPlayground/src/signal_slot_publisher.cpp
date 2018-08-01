#include "signals/signal.h"
#include <cstring>
#include <iostream>
#include "listener.h"
#include "publisher.h"


int main()
{
    Listener listener{};
    Publisher publisher{};


    // Thanks to perfect forwarding in combination with a variadic template function the same connect method
    // also works like this to connect a signal to a concrete slot
    publisher.signal.connect(&Listener::slot, &listener);
    connect(publisher.signal, &Listener::slot, &listener);
    connect(publisher.signal, [](auto const& data){ std::cout << "Lambda: "<< data.some_data << std::endl;});

    // Yields three outputs, since the publisher was connected to the listener twice and also a lambda
    publisher.DoStuff();

    return 0;
}
