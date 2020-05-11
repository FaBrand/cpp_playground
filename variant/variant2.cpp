/*This is a natural and known continuation of the std::variant/std::visitor
 * pattern called overload pattern that is also new to C++17:
 * https://dzone.com/articles/two-lines-of-code-and-three-c17-features-the-overl
 * This allowed me to remove the second interface layer in the behaviours of the
 * previous example because now each behaviour gets the data it requires. Again,
 * notice how there is not a single pointer or an abstract interface here too.*/

#include <iostream>
#include <variant>
#include <vector>

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

struct Data {
    std::string Walk = "Walking";
};
struct Data2 {
    std::string Trot = "Trotting";
};

struct Trot {
    void update(const Data2& data) { std::cout << data.Trot << std::endl; }
};
struct StaticWalk {
    void update(const Data& data) { std::cout << data.Walk << std::endl; }
};

using MyBehaviour = std::variant<Trot, StaticWalk>;

int main() {
    StaticWalk walk;
    Trot trot;
    Data data;
    Data2 data2;
    MyBehaviour behavour = trot;

    std::visit(
        overloaded{
            [&data2](Trot& beh) { beh.update(data2); },
            [&data](StaticWalk& beh) { beh.update(data); },
        },
        behavour);

    return 0;
}
