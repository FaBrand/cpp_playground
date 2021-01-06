#include <vector>
#include <ranges>
#include <iostream>


using namespace std::views;

int main()
{
    std::vector<double> currents{0.1,0.2,.13,.12,123,5};
    auto torque_constant = [](double i){ return i * 0.15; };
    auto gearbox = [](double tau_m) { return tau_m * (-1./9.) ; };

    auto joint_torque = transform(torque_constant)
        | transform(gearbox);

    for (double i : currents | joint_torque) {
        std::cout << i << ' ';
    }
    std::cout<<std::endl;
}
