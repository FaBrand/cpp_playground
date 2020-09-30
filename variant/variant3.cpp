/*Here is my daily extensions of the new 'vocabulary type' std::variant. This
 * time I anticipated that behaviours are going to be heavy of private data
 * (bad bad bad, but object oriented culture) which will make copies expensive
 * and therefore pointer semantic likely. In this example, I also introduced a
 * vector of behaviours, in case we want to call several ones within a single
 * cycle. Also, I have visually separated the lambdas from std::visit by
 * creating an overloaded visitor before the loop. Isn't that a functional
 * safety beauty? At this point, I don't see any potential issues with these
 * language features anymore. They makes code safer, more efficient, and easier
 * to read.*/

#include <iostream>
#include <variant>
#include <vector>

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

using WalkData = std::string;
using TrotData = std::string;
struct Data {
    WalkData walk = "Walking";
    TrotData trot = "Trotting";
};
struct Trot {
    void update(const TrotData& trot) { std::cout << trot << std::endl; }
};
struct StaticWalk {
    void update(const WalkData& walk) { std::cout << walk << std::endl; }
};

using MyBehaviour = std::variant<Trot*, StaticWalk*>;

int main() {
    StaticWalk walk;
    Trot trot;
    Data data;
    std::vector<MyBehaviour> behavours = {&trot, &walk};

    auto visitors =
        overloaded{[&data](Trot* beh) { beh->update(data.trot); },
                   [&data](StaticWalk* beh) { beh->update(data.walk); }};

    for (auto& behavour : behavours) {
        std::visit(visitors, behavour);
    }

    return 0;
}
