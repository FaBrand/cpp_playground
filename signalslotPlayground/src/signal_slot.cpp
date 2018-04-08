#include "signal.h"
#include <cstring>
#include <iostream>
#include "listener.h"

void Function()
{
    std::cout << "Called a function" << '\n';
}

void Function(std::string const& text)
{
    std::cout << "Called a function with argument:" << text << '\n';
}

int main()
{
    Listener listener{};

    Signal<> signal;
    signal.connect([]() { std::cout << "Called regular Lambda" << '\n'; });

    // This would be a workaround for overloaded functions.
    // The compiler can't deduce here which overload should be taken.
    // A explicit cast to the desired type would resolve this.
    signal.connect(static_cast<void (*)()>(Function));

    // This helper would do the same. It still doubles the actual type which is already
    // available in the signal instance
    signal.connect(static_cast<Signal<>::callable_t>(Function));

    signal.connect(&Listener::slot1, &listener);

    signal.emit();

    Signal<const std::string&> string_signal;
    string_signal.connect([](const auto& text) { std::cout << "Received Text:" << text << '\n'; });

    // With this example it becomes more apparent that duplicating the function signature is not desirable
    string_signal.connect(static_cast<Signal<const std::string&>::callable_t>(Function));

    string_signal.connect(&Listener::slot2, &listener);

    string_signal.emit("abc");

    return 0;
}
