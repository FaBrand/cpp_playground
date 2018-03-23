#include <iostream>
#include <si/lengths.h>

int main()
{
    meter a{1};
    kilometer c{1};
    minutes b{1};
    hours d{1};
    std::cout << "1 minute: " << b.to<seconds>() << "seconds" << '\n';
    std::cout << "1 minute: " << cast<seconds>(b) << "seconds" << '\n';
    std::cout << "1 hour: " << cast<minutes>(d) << "minutes" << '\n';
    b = d;
    std::cout << "1 hour: " << cast<minutes>(b) << "minutes" << '\n';

    seconds bd{b};
    std::cout << "1 hour: " << cast<seconds>(bd) << "seconds" << '\n';

    std::cout << "1 meter: " << cast<kilometer>(a) << "km" << '\n';
    std::cout << "1 meter: " << cast<dezimeter>(a) << "dm" << '\n';

    std::cout << "1 kilometer: " << cast<meter>(c) << "m" << '\n';
    std::cout << "1 kilometer: " << cast<dezimeter>(c) << "dm" << '\n';
    std::cout << "1 kilometer: " << c.to<dezimeter>() << "dm" << '\n';

    return 0;
}
