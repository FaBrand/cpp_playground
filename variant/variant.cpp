#include <iostream>
#include <variant>
#include <vector>

struct Data {
    std::string Trot = "Trotting";
    std::string Walk = "Walking";
};
struct Trot {
    void update(const Data& data) { update(data.Trot); }
    void update(const std::string& subset) { std::cout << subset << std::endl; }
};
struct StaticWalk {
    void update(const Data& data) { update(data.Walk); }
    void update(const std::string& subset) { std::cout << subset << std::endl; }
};

using MyBehaviour = std::variant<Trot, StaticWalk>;

int main() {
    StaticWalk walk;
    Trot trot;
    Data data;
    MyBehaviour behavour = trot;
    std::visit([&data](auto&& v) { v.update(data); }, behavour);
    // Operator is concerned, let's walk instead
    behavour = walk;
    std::visit([&data](auto&& v) { v.update(data); }, behavour);
    return 0;
}
