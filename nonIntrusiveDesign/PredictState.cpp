#include <iostream>
#include <vector>
#include <variant>

struct LongitudinalState
{
    double x;
    double dotx;
};

struct GlobalState
{
    double x;
    double y;
    double dotx;
    double doty;
};

using LongitudinalTrajectory = std::vector<LongitudinalState>;
std::vector<std::variant<LongitudinalState, GlobalState>> tube;

LongitudinalState Integrate(LongitudinalState const& state)
{
    LongitudinalState new_state{};
    new_state.x += state.x + state.dotx;
    new_state.dotx = state.dotx;
    return new_state;
};

GlobalState Integrate(GlobalState const& state)
{
    GlobalState new_state{};
    new_state.x += state.x + state.dotx;
    new_state.y += state.y + state.doty;
    new_state.dotx = state.dotx;
    new_state.doty = state.doty;
    return new_state;
};

void Integrate(LongitudinalTrajectory& trajectory, const LongitudinalState& current_state)
{
    double dt = 0.;
    for (auto& state : trajectory)
    {
        dt++;
        state.x = current_state.x + dt * current_state.dotx;
        state.dotx = current_state.dotx;
    };
};

std::ostream& operator<<(std::ostream& os, LongitudinalState const& state)
{
    return os << state.x << " " << state.dotx;
};

std::ostream& operator<<(std::ostream& os, LongitudinalTrajectory const& trajectory)
{
    for (auto const& state : trajectory)
    {
        os << state << "\n";
    }
    return os;
};

int main()
{
    // Create states and trajectories of different types
    LongitudinalState long_state{0., 1.};
    GlobalState global_state{};

    new_long_state = Predict(long_state);
    new_global_state = Predict(global_state);

    LongitudinalTrajectory trajectory(10, LongitudinalState{});
    Integrate(trajectory, long_state);
    std::cout << trajectory << "\n";

    tube.push_back(long_state);
    tube.push_back(global_state);
}
