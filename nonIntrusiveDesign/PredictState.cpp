#include <iostream>
#include <vector>

struct LongitudinalState
{
    double x;
    double dotx;
};

struct GlobalState:
{
    double x;
    double y;
    double dotx;
    double doty;
};

using LongitudinalTrajectory = std::vector<LongitudinalState>;

LongitudinalState Predict(LongitudinalState const& state)
{
    LongitudinalState new_state{};
    new_state.x += state.x + state.dotx;
    new_state.dotx = state.dotx;
    return new_state;
};

GlobalState Predict(GlobalState const& state)
{
    GlobalState new_state{};
    new_state.x += state.x + state.dotx;
    new_state.y += state.y + state.doty;
    new_state.dotx = state.dotx;
    new_state.doty = state.doty;
    return new_state;
};

std::ostream& operator<<(std::ostream& os, State const& state)
{
    return os << state.x << " " << state.y;
};

int main()
{
    LongitudinalState long_state{};
    GlobalState global_state{};

    new_long_state = Predict(long_state);
    new_global_state = Predict(global_state);

    // LongitudinalTrajectory trajectory;
    // trajectory.push_back(state);

    // Predict(trajectory);
    // Print(trajectory);
}
