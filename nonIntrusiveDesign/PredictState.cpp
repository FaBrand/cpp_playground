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

    auto new_long_state = Predict(long_state);
    auto new_global_state = Predict(global_state);

    new_long_state = long_state.Predict();
    new_global_state = global_state.Predict();
    std::vector<State> all_states;
    all_states.push_back(long_state);
    all_states.push_back(global_state);
    for (auto state : all_states)
    {
        state.Predict();
    }

    // LongitudinalTrajectory trajectory;
    // trajectory.push_back(state);

    // Predict(trajectory);
    // Print(trajectory);
}