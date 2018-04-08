#include "signal.h"
#include <cstring>
#include <iostream>

int main()
{
    Signal<> signal;
    signal.connect([]() { std::cout << "Normal Lambda" << '\n'; });
    signal.emit();

    Signal<std::string> string_signal;
    string_signal.connect([](const auto& text) { std::cout << "Received Text:" << text << '\n'; });
    string_signal.emit("abc");

    return 0;
}
